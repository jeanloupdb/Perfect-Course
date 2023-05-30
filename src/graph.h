#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


// Constante de la taille de la fenêtre et du graph
// 0.8 > SIZE_OF_GRAPH > 1.8
#define SIZE_OF_GRAPH 1.8

// Constante pour afficher les details des fonctions : 0 pour non, 1 pour oui
#define PRINT 1

#define M_PI 3.14159265358979323846

/**
 * Structure représentant un graphe.
 */
struct Graph {
    int order;                   // Nombre de noeuds dans le graphe
    struct Node *tableNodes[512]; // Tableau contenant les noeuds du graphe
};

// Prototypes des fonctions


/**
 * Structure représentant un noeud du graphe.
 */
struct Node {
    int nodeID;                // Identifiant du noeud
    int boolShelf;             // 0 si noeud extrémité d'une étagère, 1 si noeud produit, 2 si noeud entrée de l'entrepôt
    int shelf;                 // Numéro de l'étagère (valide uniquement si boolShelf est égal à 0 ou 1)
    int department;            // Numéro du rayon (valide uniquement si boolShelf est égal à 0 ou 1)
    int boolTopBottom;         // 0 si noeud intermédiaire, 1 si noeud du haut, 2 si noeud du bas
    struct Node *adjTab[10];   // Tableau contenant les noeuds adjacents
    int nbAdj;                 // Nombre de noeuds adjacents
    int X;                     // Coordonnées X du noeud
    int Y;                     // Coordonnées Y du noeud
    int productsTab[3];        // Tableau contenant les références des produits du noeud
    int nbProducts;            // Nombre de produits dans le noeud
};


/**
 * Structure représentant un article.
 */
typedef struct Article {
    int ref;             // Référence de l'article
    char* nom;           // Nom de l'article
    float prix;          // Prix de l'article
    float poids;         // Poids de l'article (en kg)
    float volume;        // Volume de l'article (en m3)
    float resistance;    // Résistance de l'article entre 0 et 1
    int frozen;          // Indicateur indiquant si l'article est congelé (1) ou non (0)
} Article;



struct eltBellman 
{
        int distance;
        int precedentNode;
};


/**
 * Creates a new graph with the specified printing option.
 *
 * @param print The printing option (0 - no printing, 1 - print node details).
 * @return The pointer to the created graph.
 */
struct Graph *createGraph(int print);

/**
 * Prints the details of a given node.
 *
 * @param node The node to be printed.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void printNode(struct Node *node, int print);

/**
 * Prints the details of a link between two nodes.
 *
 * @param node1 The first node of the link.
 * @param node2 The second node of the link.
 * @param print The printing option (0 - no printing, 1 - print link details).
 */
void printLink(struct Node *node1, struct Node *node2, int print);

/**
 * Retrieves the number of articles in the specified file.
 *
 * @param filename The name of the file.
 * @return The number of articles.
 */
int getNbArticles(char *filename);

/**
 * Creates a new node in the graph with the specified attributes.
 *
 * @param G The graph in which the node will be created.
 * @param boolShelf Indicates if the node represents a shelf (1 - true, 0 - false).
 * @param department The department of the node.
 * @param shelf The shelf number of the node.
 * @param boolTopBottom Indicates if the node is on the top or bottom level (1 - top, 0 - bottom).
 * @param X The X-coordinate of the node.
 * @param Y The Y-coordinate of the node.
 * @param p1 The first parameter of the node.
 * @param p2 The second parameter of the node.
 * @param p3 The third parameter of the node.
 * @param print The printing option (0 - no printing, 1 - print node details).
 * @return The pointer to the created node.
 */
struct Node *createNode(struct Graph *G, int boolShelf, int department, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3, int print);

/**
 * Retrieves the node representing a shelf in the specified department and shelf number.
 *
 * @param G The graph containing the nodes.
 * @param department The department of the shelf.
 * @param shelf The shelf number.
 * @return The pointer to the node representing the shelf.
 */
struct Node *getNodeShelf(struct Graph *G, int department, int shelf);

/**
 * Retrieves the intermediate node in the specified department and top/bottom level.
 *
 * @param G The graph containing the nodes.
 * @param department The department of the intermediate node.
 * @param boolTopBottom Indicates if the node is on the top or bottom level (1 - top, 0 - bottom).
 * @return The pointer to the intermediate node.
 */
struct Node *getNodeIntermediate(struct Graph *G, int department, int boolTopBottom);

/**
 * Creates a link between two nodes in the graph.
 *
 * @param node1 The first node.
 * @param node2 The second node.
 * @param print The printing option (0 - no printing, 1 - print link details).
 */
void createLink(struct Node *node1, struct Node *node2, int print);

/**
 * Reads the data from the specified file and stores it in the graph.
 *
 * @param G The graph in which the data will be stored.
 * @param filename The name of the file.
 * @param print The printing option (0 - no printing, 1 - print node details).
 * @return The ref of the starting node.
 */
int FileRead(struct Graph *G, char *filename, int print);

/**
 * Calculates the distance between two nodes in the graph.
 *
 * @param graph The graph containing the nodes.
 * @param nd1 The index of the first node.
 * @param nd2 The index of the second node.
 * @return The distance between the two nodes.
 */
int distance(struct Graph *graph, int nd1, int nd2);

/**
 * Executes the Bellman-Ford algorithm on the graph starting from the specified source node.
 *
 * @param graph The graph on which the algorithm will be executed.
 * @param sourceNode The index of the source node.
 * @param tab The array to store the results of the algorithm.
 */
void algoBellman(struct Graph *graph, int sourceNode, struct eltBellman *tab);

/**
 * Prints the results of the Bellman-Ford algorithm.
 *
 * @param tab The array containing the results of the algorithm.
 * @param order The order of the graph.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void printBellman(struct eltBellman *tab, int order, int print);

/**
 * Calculates the path from the source node to the target node based on the results of the Bellman-Ford algorithm.
 *
 * @param tabBellman The array containing the results of the algorithm.
 * @param sourceNode The index of the source node.
 * @param targetNode The index of the target node.
 * @param pathToTarget The array to store the path from the source to the target node.
 * @param nbPathNodes The number of nodes in the path.
 */
void pathToTarget(struct eltBellman *tabBellman, int sourceNode, int targetNode, int *pathToTarget, int *nbPathNodes);

/**
 * Prints the path from the source node to the target node.
 *
 * @param pathToTarget The array containing the path from the source to the target node.
 * @param nbPathNodes The number of nodes in the path.
 * @param sourceNode The index of the source node.
 * @param targetNode The index of the target node.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void printPathToTarget(int *pathToTarget, int nbPathNodes, int sourceNode, int targetNode, int print);

/**
 * Retrieves the number of articles in the panier (cart).
 *
 * @param panier The array representing the cart.
 * @param nbArticles The total number of articles.
 * @return The number of articles in the cart.
 */
int getNbArticlesInPanier(int *panier, int nbArticles);

/**
 * Prints the contents of the panier (cart).
 *
 * @param graph The graph containing the nodes.
 * @param panier The array representing the cart.
 * @param catalogue The array containing the article data.
 * @param nbArticles The total number of articles.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void print_panier(struct Graph *graph, int *panier, Article *catalogue, int nbArticles, int print);

/**
 * Converts the reference to the corresponding node index in the graph.
 *
 * @param G The graph containing the nodes.
 * @param ref The reference value.
 * @return The index of the node.
 */
int ref_to_node(struct Graph *G, int ref);

/**
 * Compares two articles based on their scores.
 *
 * @param a The first article.
 * @return The comparison result.
 */
int scoreArticle(const void *a);

/**
 * Fills the cart with articles based on the specified file.
 *
 * @param panier The array representing the cart.
 * @param file The name of the file.
 * @param nbArticles The total number of articles.
 */
void remplir_panier(int *panier, char *file, int nbArticles);

/**
 * Calculates the shortest path from the source node to the target node, considering the articles in the cart.
 *
 * @param graph The graph containing the nodes.
 * @param panier The array representing the cart.
 * @param catalogue The array containing the article data.
 * @param sourceNode The index of the source node.
 * @param tab The array containing the results of the Bellman-Ford algorithm.
 * @param path2 The array to store the shortest path.
 * @param NbArticlesInPanier The number of articles in the cart.
 * @param nbArticles The total number of articles.
 * @param print The printing option (0 - no printing, 1 - print node details).
 * @return The shortest path as an array.
 */
int *plus_court_chemin(struct Graph *graph, int *panier, Article *catalogue, int sourceNode, struct eltBellman *tab, int *path2, int NbArticlesInPanier, int nbArticles, int print);

/**
 * Prints the path based on the given graph, path, cart, catalogue, and printing option.
 *
 * @param graph The graph containing the nodes.
 * @param path The array representing the path.
 * @param panier The array representing the cart.
 * @param catalogue The array containing the article data.
 * @param nbArticles The total number of articles.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void print_path(struct Graph *graph, int *path, int *panier, struct Article *catalogue, int nbArticles, int print);

/**
 * Creates a path based on the given graph, path, cart, catalogue, Bellman-Ford results, number of articles, and printing option.
 *
 * @param G The graph containing the nodes.
 * @param path The array representing the path.
 * @param panier The array representing the cart.
 * @param catalogue The array containing the article data.
 * @param tab The array containing the results of the Bellman-Ford algorithm.
 * @param nbArticles The total number of articles.
 * @param print The printing option (0 - no printing, 1 - print node details).
 * @return The created path as an array.
 */
int *creer_chemin(struct Graph *G, int *path, int *panier, struct Article *catalogue, struct eltBellman *tab, int nbArticles, int print);

/**
 * Fills the catalogue with article data from the specified file.
 *
 * @param catalogue The array to store the article data.
 * @param nom_fichier The name of the file.
 */
void remplir_catalogue(Article *catalogue, char *nom_fichier);

/**
 * Prints the contents of the catalogue.
 *
 * @param catalogue The array containing the article data.
 * @param nbArticles The total number of articles.
 * @param print The printing option (0 - no printing, 1 - print node details).
 */
void print_catalogue(Article *catalogue, int nbArticles, int print);

/**
 * Draws a square at the specified position with the specified size.
 *
 * @param x The X-coordinate of the square's top-left corner.
 * @param y The Y-coordinate of the square's top-left corner.
 * @param size The size of the square.
 * @param bool_panier Indicates if the square represents a cart item (1 - true, 0 - false).
 */
void drawSquare(int x, int y, int size, int bool_panier);

/**
 * Draws a line between two points.
 *
 * @param x1 The X-coordinate of the first point.
 * @param y1 The Y-coordinate of the first point.
 * @param x2 The X-coordinate of the second point.
 * @param y2 The Y-coordinate of the second point.
 */
void drawLine(int x1, int y1, int x2, int y2);

/**
 * Draws an arrow between two points.
 *
 * @param x1 The X-coordinate of the starting point.
 * @param y1 The Y-coordinate of the starting point.
 * @param x2 The X-coordinate of the ending point.
 * @param y2 The Y-coordinate of the ending point.
 */
void drawArrow(int x1, int y1, int x2, int y2);

/**
 * Draws the iterator path at the specified position.
 *
 * @param x The X-coordinate of the iterator path.
 * @param y The Y-coordinate of the iterator path.
 * @param itterator_path The iterator path value.
 */
void draw_itterator_path(int x, int y, int itterator_path);

/**
 * Displays the OpenGL window.
 */
void display();

/**
 * Initializes the OpenGL environment.
 */
void init();

/**
 * Reshapes the OpenGL window to the specified width and height.
 *
 * @param w The new width of the window.
 * @param h The new height of the window.
 */
void reshape(int w, int h);

/**
 * The main function for the OpenGL program.
 *
 * @return The program exit code.
 */
int main_openGL();






#endif // __GRAPH_H__
