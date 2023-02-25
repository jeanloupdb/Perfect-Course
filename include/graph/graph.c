#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Création d'un graphe
Graph* create_graph(int order) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->order = order;
    graph->adj_list = calloc(order * order, sizeof(int));
    if (graph->adj_list == NULL) {
        free(graph);
        return NULL;
    }
    graph->edge_costs = calloc(order * order, sizeof(float));
    if (graph->edge_costs == NULL) {
        free(graph->adj_list);
        free(graph);
        return NULL;
    }
    graph->article_index = calloc(order, sizeof(Article));
    if (graph->article_index == NULL) {
        free(graph->edge_costs);
        free(graph->adj_list);
        free(graph);
        return NULL;
    }
    graph->nodes_coord = calloc(order, sizeof(float*));
    if (graph->nodes_coord == NULL) {
        free(graph->article_index);
        free(graph->edge_costs);
        free(graph->adj_list);
        free(graph);
        return NULL;
    }
    for (int i = 0; i < order; i++) {
        graph->nodes_coord[i] = calloc(2, sizeof(float));
        if (graph->nodes_coord[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(graph->nodes_coord[j]);
            }
            free(graph->nodes_coord);
            free(graph->article_index);
            free(graph->edge_costs);
            free(graph->adj_list);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

// Destruction d'un graphe
void destroy_graph(Graph* graph) {
    for (int i = 0; i < graph->order; i++) {
        free(graph->nodes_coord[i]);
    }
    free(graph->nodes_coord);
    free(graph->article_index);
    free(graph->edge_costs);
    free(graph->adj_list);
    free(graph);
}

// Ajout d'une arête entre deux sommets avec un coût associé
void add_edge(Graph* graph, int u, int v, float cost) {
    graph->adj_list[u * graph->order + v] = 1;
    graph->adj_list[v * graph->order + u] = 1;
    graph->edge_costs[u * graph->order + v] = cost;
    graph->edge_costs[v * graph->order + u] = cost;
}

// Ajout d'un article dans le graphe
void add_article(Graph* graph, char* nom, float prix, float poids, float volume, float resistance) {
    for (int i = 0; i < graph->order; i++) {
        if (graph->article_index[i].nom == NULL) {
            graph->article_index[i].nom = malloc(strlen(nom) + 1);
            if (graph->article_index[i].nom == NULL) {
                return;
            }
            strcpy(graph->article_index[i].nom, nom);
            graph->article_index[i].prix = prix;
            graph->article_index[i].poids = poids;
            graph->article_index[i].volume = volume;
            graph->article_index[i].resistance = resistance;
            break;
        }
    }
}

// Ajout d'un sommet avec des coordonnées
void add_node(Graph* graph, float x, float y) {
    for (int i = 0; i < graph->order; i++) {
        if (graph->nodes_coord[i][0] == 0 && graph->nodes_coord[i][1] == 0) {
            graph->nodes_coord[i][0] = x;
            graph->nodes_coord[i][1] = y;
            break;
        }
    }
}