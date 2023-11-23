#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Vertex;
typedef struct {
  int u;
  int v;
} Edge;

typedef struct node *link;
struct node {
  Vertex w;
  link next;
};

struct graph {
  int V;
  int E;
  link *adj;
  int *discovery;
  int *low;
  int *onStack;
  int *component;
  int time;
};

typedef struct graph *Graph;

typedef struct {
  int *vertices;
  int count;
} StronglyConnectedComponent;

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
  Graph G = Malloc(sizeof(*G));
  G->V = V;
  G->E = 0;
  G->adj = Malloc(V * sizeof(link));
  G->discovery = Malloc(V * sizeof(int));
  G->low = Malloc(V * sizeof(int));
  G->onStack = Malloc(V * sizeof(int));
  G->component = Malloc(V * sizeof(int));
  G->time = 0;

  for (Vertex u = 0; u < V; u++) {
    G->adj[u] = NULL;
    G->onStack[u] = 0;
  }

  return G;
}

void graph_insert_edge(Graph G, Edge e) {
  assert(G);
  assert(e.u >= 0 && e.u < G->V);
  assert(e.v >= 0 && e.v < G->V);
  G->adj[e.u] = list_insert(G->adj[e.u], e.v);
  G->E += 1;
}

Edge edge(int u, int v) {
  Edge e = {u, v};
  return e;
}

StronglyConnectedComponent *create_scc(int size) {
  StronglyConnectedComponent *scc = Malloc(sizeof(StronglyConnectedComponent));
  scc->vertices = Malloc(size * sizeof(int));
  scc->count = 0;
  return scc;
}

void destroy_scc(StronglyConnectedComponent *scc) {
  free(scc->vertices);
  free(scc);
}

int compare_integers(const void *a, const void *b) {
  return (*(int *)a - *(int *)b);
}

// me ajuda a printar as regioes altamente conectadas em ordem crescente
void print_scc(StronglyConnectedComponent *scc) {
  qsort(scc->vertices, scc->count, sizeof(int), compare_integers);
  for (int i = 0; i < scc->count; i++) {
    printf("%d ", scc->vertices[i]);
  }
  printf("\n");
}

void tarjan_dfs(Graph G, Vertex v, int *stack, int *top,
                StronglyConnectedComponent **sccList, int *sccCount) {
  G->discovery[v] = G->low[v] = ++G->time;
  stack[++(*top)] = v;
  G->onStack[v] = 1;

  for (link p = G->adj[v]; p; p = p->next) {
    Vertex w = p->w;
    if (G->discovery[w] == 0) {
      tarjan_dfs(G, w, stack, top, sccList, sccCount);
      G->low[v] = G->low[v] < G->low[w] ? G->low[v] : G->low[w];
    } else if (G->onStack[w]) {
      G->low[v] = G->low[v] < G->discovery[w] ? G->low[v] : G->discovery[w];
    }
  }

  if (G->discovery[v] == G->low[v]) {
    StronglyConnectedComponent *scc = create_scc(G->V);
    (*sccCount)++;
    while (*top >= 0 && stack[*top] != v) {
      Vertex w = stack[(*top)--];
      G->onStack[w] = 0;
      G->component[w] = G->time;
      scc->vertices[scc->count++] = w;
    }
    Vertex w = stack[(*top)--];
    G->onStack[w] = 0;
    G->component[w] = G->time;
    scc->vertices[scc->count++] = w;
    sccList[(*sccCount) - 1] = scc;
  }
}

void tarjan(Graph G) {
  int *stack = Malloc(G->V * sizeof(int));
  int top = -1;

  StronglyConnectedComponent **sccList =
      Malloc(G->V * sizeof(StronglyConnectedComponent *));
  int sccCount = 0;

  for (Vertex v = 0; v < G->V; v++) {
    if (G->discovery[v] == 0) {
      tarjan_dfs(G, v, stack, &top, sccList, &sccCount);
    }
  }

  free(stack);

  if (sccCount == 1) {
    printf("Ha 1 regiao altamente conectada:\n");
    for (int i = 0; i < sccCount; i++) {
      print_scc(sccList[i]);
      destroy_scc(sccList[i]);
    }
    return;
  }

  printf("Ha %d regioes altamente conectadas:\n", sccCount);
  for (int i = 0; i < sccCount; i++) {
    print_scc(sccList[i]);
    destroy_scc(sccList[i]);
  }

  free(sccList);
}

int main() {
  int V, E;
  scanf("%d %d", &V, &E);

  Graph G = graph(V);

  for (int i = 0; i < E; i++) {
    Vertex u, v;
    scanf("%d %d", &u, &v);
    graph_insert_edge(G, edge(u, v));
  }

  tarjan(G);

  return 0;
}
