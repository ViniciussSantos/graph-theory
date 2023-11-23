#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Vertex;
typedef struct {
  int u;
  int v;
} Edge;
typedef struct graph *Graph;
typedef struct node *link;

struct node {
  Vertex w;
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

link list_insert(link head, Vertex w) {
  link p = Malloc(sizeof(*p));
  p->w = w;
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

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);

  G->adj[e.u] = list_insert(G->adj[e.u], e.v);
  G->adj[e.v] = list_insert(G->adj[e.v], e.u);
  G->E += 1;
}

Edge edge(int u, int v) {
  Edge e = {u, v};
  return e;
}
int main(int argc, char const *argv[]) {

  int V, E;
  scanf("%d %d", &V, &E);

  Graph G = graph(V);

  for (int i = 0; i < E; i++) {
    Vertex u, v;
    scanf("%d %d", &u, &v);
    graph_insert_edge(G, edge(u, v));
  }

  return 0;
}
