#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>

struct Graph
{
    int order;
    struct Node *tableNodes[512];
};


// Node: structure d'un noeud, contient les informations sur une etagere ou un noeud intermediaire
struct Node
{
    int nodeID; // Identifiant du noeud
    int boolShelf; // 0 si noeud extremite d'une etagere, 1 si noeud produit, 2 si noeud entree de l'entrepot
    int shelf; // numero de l'etagere
    int department; // numero du rayon
    int boolTopBottom; // 0 si noeud intermediaire, 1 si noeud du haut, 2 si noeud du bas
    struct Node *adjTab[10]; // Tableau contenant les noeuds adjacents
    int nbAdj; // Nombre de noeuds adjacents
    int X; // Coordonnees du noeud
    int Y; // Coordonnees du noeud
    int productsTab[3]; // Tableau contenant les references des produits du noeud
    int nbProducts;
};

// Définition de la structure d'un article
typedef struct Article {
    int ref;           // Référence de l'article
    char* nom;         // Nom de l'article
    float prix;        // Prix de l'article
    float poids;       // Poids de l'article (en kg)
    float volume;      // Volume de l'article (en m3)
    float resistance;  // Résistance de l'article entre 0 et 1
    int frozen;      // 1 si l'article est congelé, 0 sinon
} Article;


struct eltBellman 
{
        int distance;
        int precedentNode;
};


// Prototypes des fonctions

/**
 * Affiche les résultats du tableau de Bellman.
 *
 * @param tab Le tableau de Bellman contenant les distances et prédécesseurs des nœuds.
 * @param order Le nombre total de nœuds dans le graphe.
 */
void printBellman(struct eltBellman tab[], int order);
/**
 * Trouve le chemin du nœud source au nœud cible à partir du tableau de Bellman.
 *
 * @param tabBellman Le tableau de Bellman contenant les distances et prédécesseurs des nœuds.
 * @param sourceNode L'identifiant du nœud source.
 * @param targetNode L'identifiant du nœud cible.
 * @param pathToTarget Le tableau qui va contenir les nœuds du chemin du source au cible.
 * @param nbPathNodes Un pointeur vers la variable qui va contenir le nombre de nœuds dans le chemin.
 */
void pathToTarget(struct eltBellman tabBellman[], int sourceNode, int targetNode, int pathToTarget[], int *nbPathNodes);
/**
 * Affiche le plus court chemin entre un nœud source et un nœud cible.
 *
 * @param graph Le graphe contenant les nœuds.
 * @param pathToTarget Le tableau des nœuds formant le plus court chemin.
 * @param nbPathNodes Le nombre de nœuds dans le plus court chemin.
 * @param sourceNode L'identifiant du nœud source.
 * @param targetNode L'identifiant du nœud cible.
 */
void printPathToTarget(struct Graph *graph, int pathToTarget[], int nbPathNodes, int sourceNode, int targetNode);
/**
 * Trouve le plus court chemin en tenant compte de l'ordre de priorité des articles dans le panier.
 *
 * @param graph       Le graphe représentant le magasin.
 * @param panier      Le panier contenant les articles à récupérer.
 * @param catalogue   Le catalogue des articles avec leurs caractéristiques.
 * @param sourceNode  Le nœud source à partir duquel commencer la recherche.
 * @param tab         Le tableau des distances et des prédécesseurs généré par l'algorithme de Bellman.
 * @param path2       Le tableau qui stockera le chemin le plus court.
 * @param nbart       Le nombre d'articles dans le panier.
 */
void plus_court_chemin(struct Graph *graph, int panier[], Article catalogue[], int sourceNode, struct eltBellman tab[], int path2[], int nbart);
/**
 * Compare deux articles en utilisant une formule personnalisée pour déterminer leur ordre de priorité.
 * Cette fonction est utilisée pour trier les articles dans le panier en fonction de leurs caractéristiques.
 *
 * @param a Le premier article à comparer.
 * @param b Le deuxième article à comparer.
 * @return Un entier positif si l'article a a une priorité supérieure à l'article b,
 *         un entier négatif si l'article a a une priorité inférieure à l'article b,
 *         et zéro si les articles ont la même priorité.
 */
int compareArticles(const void *a, const void *b);
/**
 * Transforme une référence d'article en identifiant de nœud correspondant dans le graphe.
 *
 * @param G Le graphe contenant les nœuds.
 * @param ref La référence de l'article.
 * @return L'identifiant du nœud correspondant à la référence de l'article,
 *         ou -1 si aucun nœud ne correspond à la référence donnée.
 */
int ref_to_node(struct Graph *G, int ref);
/**
 * Affiche le contenu du panier avec les informations sur chaque article.
 *
 * @param graph Le graphe contenant les nœuds.
 * @param panier Le tableau représentant le panier.
 * @param catalogue Le tableau contenant les articles du magasin.
 */
void print_panier(struct Graph *graph, int panier[], Article catalogue[]);
/**
 * Affiche les informations d'un nœud du graphe.
 *
 * @param node Le nœud à afficher.
 */
void printNode(struct Node *node);
/**
 * Affiche la liaison entre deux nœuds du graphe.
 *
 * @param node1 Le premier nœud de la liaison.
 * @param node2 Le deuxième nœud de la liaison.
 */
void printLink(struct Node *node1, struct Node *node2);
/**
 * Crée un nouveau nœud du graphe avec les paramètres spécifiés.
 *
 * @param boolShelf     Indique le type de nœud : 0 pour extrémité d'étagère, 1 pour nœud produit, 2 pour entrée d'entrepôt.
 * @param aisle         Le numéro de l'allée à laquelle le nœud appartient.
 * @param shelf         Le numéro de l'étagère à laquelle le nœud appartient.
 * @param boolTopBottom Indique si le nœud est en haut (1) ou en bas (0) de l'étagère.
 * @param X             La coordonnée X du nœud.
 * @param Y             La coordonnée Y du nœud.
 * @param p1            La référence du premier produit associé au nœud.
 * @param p2            La référence du deuxième produit associé au nœud.
 * @param p3            La référence du troisième produit associé au nœud.
 * @param print         Indique si le nœud doit être imprimé (1) ou non (0).
 * @return              Le nouveau nœud créé.
 */
struct Node *createNode(int boolShelf, int aisle, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3, int print);
/**
 * Renvoie le nœud d'étagère correspondant à l'étage et à la colonne spécifiés.
 *
 * @param department Le numéro de l'étage.
 * @param shelf      Le numéro de la colonne.
 * @return           Le nœud d'étagère correspondant, ou NULL s'il n'est pas trouvé.
 */
struct Node *getNodeShelf(int department, int shelf);

/**
 * Renvoie le nœud intermédiaire entre deux nœuds d'étagère du même étage.
 *
 * @param department    Le numéro de l'étage.
 * @param boolTopBottom Indique si le nœud recherché est en haut (1) ou en bas (0) de l'étagère.
 * @return              Le nœud intermédiaire correspondant, ou NULL s'il n'est pas trouvé.
 */
struct Node *getNodeIntermediate(int department, int boolTopBottom);
/**
 * Crée un lien entre deux nœuds.
 *
 * @param node1 Le premier nœud.
 * @param node2 Le deuxième nœud.
 * @param print Indique si les étapes de création du lien doivent être affichées (1) ou non (0).
 */
void createLink(struct Node *node1, struct Node *node2, int print);

/**
 * Crée le graphe représentant le plan de stockage à partir d'un fichier.
 *
 * @param filename Le nom du fichier contenant les informations du plan de stockage.
 * @param print    Indique si les étapes de création du graphe doivent être affichées (1) ou non (0).
 * @return         Le graphe créé.
 */
struct Graph *FileRead(char *filename, int print);



void init();

#endif // __GRAPH_H__
