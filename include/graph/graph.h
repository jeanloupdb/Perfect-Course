#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "article.h"

// Définition de la structure d'un graphe
typedef struct Graph {
    int order;                    // Nombre de sommets dans le graphe
    int* adj_list;                // Liste d'adjacence
    float* edge_costs;            // Coûts des arêtes
    Article* article_index;       // Liste des articles avec leurs indices correspondant dans le graphe
    float** nodes_coord;          // Coordonnées des sommets
} Graph;

// Création d'un graphe
Graph* create_graph(int order);

// Destruction d'un graphe
void destroy_graph(Graph* graph);

// Ajout d'une arête entre deux sommets avec un coût associé
void add_edge(Graph* graph, int u, int v, float cost);

// Ajout d'un article dans le graphe
void add_article(Graph* graph, char* nom, float prix, float poids, float volume, float resistance);

// Ajout d'un sommet avec des coordonnées
void add_node(Graph* graph, float x, float y);

// Vérifie si deux sommets sont adjacents
bool is_adjacent(Graph* graph, int u, int v);

#endif /* GRAPH_H */
