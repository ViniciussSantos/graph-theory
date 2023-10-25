#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool hasCycle = false;
typedef int Vertex;
typedef struct {
  int u;
  int v;
} Edge;
typedef struct graph *Graph;
typedef struct node *link;

struct node {
  Vertex w;
  // -1 = nao visitado, 0 = azul, 1 = vermelho
  int color;
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
  p->color = -1;
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

int get_number_of_blue(Graph G) {
  assert(G);

  int blue = 0;
  for (Vertex u = 0; u < G->V; u++) {
    if (G->adj[u]->color == 0)
      blue++;
  }

  return blue;
}

int *graph_get_blue_vertex(Graph G) {
  assert(G);

  int *blue_vertices = Malloc(get_number_of_blue(G) * sizeof(int));

  int i = 0;
  for (Vertex u = 0; u < G->V; u++) {
    if (G->adj[u]->color == 0) {
      blue_vertices[i] = u;
      i++;
    }
  }

  return blue_vertices;
}

int get_number_of_red(Graph G) {
  assert(G);

  int red = 0;
  for (Vertex u = 0; u < G->V; u++) {
    if (G->adj[u]->color == 1)
      red++;
  }

  return red;
}

int *graph_get_red_vertex(Graph G) {
  assert(G);

  int *red_vertices = Malloc(get_number_of_red(G) * sizeof(int));

  int i = 0;
  for (Vertex u = 0; u < G->V; u++) {
    if (G->adj[u]->color == 1) {
      red_vertices[i] = u;
      i++;
    }
  }

  return red_vertices;
}

void dfs(Graph G, Vertex v, int color, Vertex pred, Vertex *cycle,
         int *cycle_length) {
  if (G->adj[v] == NULL)
    return;

  // TODO: fazer com que vertices de grau 1 nao sejam inseridos no ciclo
  if (G->adj[v]->color != -1) {
    if (G->adj[v]->color != color) {

      hasCycle = true;

      // ciclo impar
      printf("GUERRA!\n");
      // interessante notar que o meu algoritmo sempre imprime o maior ciclo
      // impar possivel
      // no exemplo do enunciado, existem 2 ciclos impares, mas o meu algoritmo
      // imprime o maior
      for (int i = 0; i < *cycle_length; i++) {
        printf("%d ", cycle[i]);
      }
      printf("%d\n", v);

      return;
    }
    return;
  }

  struct node *p = G->adj[v];
  G->adj[v]->color = color;
  cycle[(*cycle_length)] = v;
  (*cycle_length)++;

  while (p != NULL) {

    // evita voltar para o vertice anterior
    // e contar o mesmo vertice duas vezes no ciclo
    if (p->w == pred) {
      p = p->next;
      continue;
    }

    if (hasCycle)
      return;

    dfs(G, p->w, color == 0 ? 1 : 0, v, cycle, cycle_length);
    p = p->next;
  }
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

  // um grafo nao pode ser bipartido se tiver um ciclo impar
  // cycle guarda os vertices do ciclo impar
  Vertex *cycle = Malloc(V * sizeof(Vertex));
  int cycle_length = 0;

  // colorir o grafo começando pelo vertice 0 com a cor azul
  dfs(G, 0, 0, -1, cycle, &cycle_length);

  if (hasCycle)
    return 0;

  printf("PAZ!\n");

  int *blue_vertices = graph_get_blue_vertex(G);
  int blue_size = get_number_of_blue(G);
  int *red_vertices = graph_get_red_vertex(G);
  int red_size = get_number_of_red(G);

  for (int i = 0; i < blue_size; i++) {
    printf("%d%s", blue_vertices[i], i == blue_size - 1 ? "\n" : " ");
  }

  for (int i = 0; i < red_size; i++) {
    printf("%d%s", red_vertices[i], i == red_size - 1 ? "\n" : " ");
  }

  return 0;
}
