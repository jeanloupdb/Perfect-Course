#include <stdio.h>
#include "graph/graph.h"

int main() {
    Graph* graph = create_graph(5);

    add_node(graph, 0.0, 0.0);
    add_node(graph, 1.0, 0.0);
    add_node(graph, 1.0, 1.0);
    add_node(graph, 0.0, 1.0);

    add_edge(graph, 0, 1, 1.0);
    add_edge(graph, 1, 2, 2.0);
    add_edge(graph, 2, 3, 3.0);
    add_edge(graph, 3, 0, 4.0);

    printf("Number of nodes: %d\n", graph->order);
    printf("Nodes:\n");
    for (int i = 0; i < graph->order; i++) {
        printf("%d: (%f, %f)\n", i, graph->nodes_coord[i][0], graph->nodes_coord[i][1]);
    }

    printf("Edges:\n");
    for (int u = 0; u < graph->order; u++) {
        for (int v = 0; v < graph->order; v++) {
            if (is_adjacent(graph, u, v)) {
                printf("(%d, %d): %f\n", u, v, graph->edge_costs[u * graph->order + v]);
            }
        }
    }

    destroy_graph(graph);

    return 0;
}
