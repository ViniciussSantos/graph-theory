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

/* Função auxiliar para sort_edges()
 * Recebe edges[ini..mid] e edges[mid+1..end] já ordenados e faz com que
 * edges[ini..end] fique totalmente ordenado */
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

/* Implementação do MergeSort para ordenar um vetor de arestas com pesos.
 * Vai ordenar o vetor edges[ini..end] (portanto o primeiro e último índices
 * devem ser elementos válidos) */
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

int main(int argc, char const *argv[]) {

  int V, E;
  scanf("%d %d", &V, &E);

  Graph G = graph(V);

  for (int i = 0; i < E; i++) {
    Vertex u, v;
    double w;
    scanf("%d %d %lf", &u, &v, &w);
    graph_insert_edge(G, wedge(u, v, w));
  }

  return 0;
}
