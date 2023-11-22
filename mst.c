#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Vertex;
typedef struct {
  Vertex u;
  Vertex v;
  double weight;
} WEdge;
typedef struct graph *Graph;
typedef struct node *link;

void merge_edges(WEdge *edges, int ini, int mid, int end) {
  WEdge *A = malloc((mid - ini + 1) * sizeof(*A));
  WEdge *B = malloc((end - mid) * sizeof(*B));

  int i, j, k;

  for (i = ini; i <= mid; i++)
    A[i - ini] = edges[i];
  for (i = mid + 1; i <= end; i++)
    B[i - mid - 1] = edges[i];

  i = 0;
  j = 0;
  k = ini;
  while (i < (mid - ini + 1) && j < (end - mid)) {
    if (A[i].weight < B[j].weight)
      edges[k++] = A[i++];
    else
      edges[k++] = B[j++];
  }
  while (i < (mid - ini + 1))
    edges[k++] = A[i++];
  while (j < (end - mid))
    edges[k++] = B[j++];

  free(A);
  free(B);
}

void sort_edges(WEdge *edges, int ini, int end) {
  if (ini < end) {
    int mid = (ini + end) / 2;
    sort_edges(edges, ini, mid);
    sort_edges(edges, mid + 1, end);
    merge_edges(edges, ini, mid, end);
  }
}

struct node {
  Vertex w;
  double weight;

  link next;
};

struct graph {
  int V;
  int E;
  link *adj;
};

void *Malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr)
    abort();
  return ptr;
}

link list_insert(link head, Vertex w, double weight) {
  link p = Malloc(sizeof(*p));
  p->w = w;
  p->weight = weight;
  p->next = head;
  return p;
}

link list_remove(link head, Vertex w) {
  if (head == NULL)
    return NULL;

  if (head->w == w) {
    link p = head->next;
    free(head);
    return p;
  } else {
    head->next = list_remove(head->next, w);
    return head;
  }
}

link list_destroy(link head) {
  if (head) {
    list_destroy(head->next);
    free(head);
  }
  return NULL;
}

Graph graph(int V) {
  assert(V > 0);

  // http://color-faq.com/aryptr/fn33.html
  Graph G = Malloc(sizeof(*G));

  G->V = V;
  G->E = 0;
  G->adj = Malloc(V * sizeof(link));

  for (Vertex u = 0; u < V; u++)
    G->adj[u] = NULL;

  return G;
}

void graph_insert_edge(Graph G, WEdge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);

  G->adj[e.u] = list_insert(G->adj[e.u], e.v, e.weight);
  G->adj[e.v] = list_insert(G->adj[e.v], e.u, e.weight);
  G->E += 1;
}

WEdge wedge(Vertex u, Vertex v, double weight) {
  WEdge e = {u, v, weight};
  return e;
}

// union-find
int find_set(int *parent, int i) {
  if (parent[i] == i)
    return i;
  else
    return find_set(parent, parent[i]);
}

void union_sets(int *parent, int i, int j) {
  int set_i = find_set(parent, i);
  int set_j = find_set(parent, j);
  parent[set_i] = set_j;
}

void kruskal(Graph G, WEdge *edges) {
  int *parent = malloc(G->V * sizeof(int));
  for (int i = 0; i < G->V; i++)
    parent[i] = i;

  sort_edges(edges, 0, G->E - 1);

  double total_weight = 0.0;

  for (int i = 0; i < G->E; i++) {
    Vertex u = edges[i].u;
    Vertex v = edges[i].v;
    double weight = edges[i].weight;

    if (find_set(parent, u) != find_set(parent, v)) {
      printf("%d %d\n", u, v);
      total_weight += weight;
      union_sets(parent, u, v);
    }
  }
  printf("%.2lf\n", total_weight);
  free(parent);
}

int main(int argc, char const *argv[]) {

  int V, E;
  scanf("%d %d", &V, &E);

  Graph G = graph(V);
  WEdge *edges = malloc(E * sizeof(WEdge));

  for (int i = 0; i < E; i++) {
    Vertex u, v;
    double w;
    scanf("%d %d %lf", &u, &v, &w);
    graph_insert_edge(G, wedge(u, v, w));
    edges[i] = wedge(u, v, w);
  }

  kruskal(G, edges);

  return 0;
}
