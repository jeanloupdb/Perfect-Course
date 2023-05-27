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


// Prototypes des fonctions

/**
 * Affiche les résultats du tableau de Bellman.
 *
 * @param tab Le tableau de Bellman contenant les distances et prédécesseurs des nœuds.
 * @param order Le nombre total de nœuds dans le graphe.
 */
void printBellman(struct eltBellman tab[], int order, int print);
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
 * @param pathToTarget Le tableau des nœuds formant le plus court chemin.
 * @param nbPathNodes Le nombre de nœuds dans le plus court chemin.
 * @param sourceNode L'identifiant du nœud source.
 * @param targetNode L'identifiant du nœud cible.
 */
void printPathToTarget(int pathToTarget[], int nbPathNodes, int sourceNode, int targetNode, int print);
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
void plus_court_chemin(struct Graph *graph, int panier[], Article catalogue[], int sourceNode, struct eltBellman tab[], int path2[], int nbart, int print);

/**
 * Calcule le score d'un article en utilisant des facteurs de pondération pour les différents paramètres.
 *
 * @param a Pointeur générique vers l'article dont on souhaite calculer le score.
 * @return Le score de l'article.
 */
int scoreArticle(const void *a);


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
void print_panier(struct Graph *graph, int panier[], Article catalogue[], int print);
/**
 * Affiche les informations d'un nœud du graphe.
 *
 * @param node Le nœud à afficher.
 */
void printNode(struct Node *node, int print);
/**
 * Affiche la liaison entre deux nœuds du graphe.
 *
 * @param node1 Le premier nœud de la liaison.
 * @param node2 Le deuxième nœud de la liaison.
 */
void printLink(struct Node *node1, struct Node *node2, int print);

/**
 * Crée un nouveau nœud dans un graphe.
 *
 * @param G                Le graphe dans lequel créer le nœud.
 * @param boolShelf        Indique le type de nœud : 0 pour extrémité d'une étagère, 1 pour produit, 2 pour entrée de l'entrepôt.
 * @param aisle            Le numéro de l'allée du nœud (valide uniquement si boolShelf est égal à 0 ou 1).
 * @param shelf            Le numéro de l'étagère du nœud (valide uniquement si boolShelf est égal à 0 ou 1).
 * @param boolTopBottom    Indique la position verticale du nœud : 0 pour intermédiaire, 1 pour le haut, 2 pour le bas.
 * @param X                La coordonnée X du nœud.
 * @param Y                La coordonnée Y du nœud.
 * @param p1               La référence du premier produit du nœud.
 * @param p2               La référence du deuxième produit du nœud.
 * @param p3               La référence du troisième produit du nœud.
 * @param print            Un entier indiquant si l'impression est activée (1) ou désactivée (0).
 * @return Le nœud nouvellement créé.
 */
struct Node *createNode(struct Graph *G, int boolShelf, int aisle, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3, int print);


/**
 * Renvoie le nœud d'étagère correspondant à l'étage et à la colonne spécifiés.
 *
 * @param department Le numéro de l'étage.
 * @param shelf      Le numéro de la colonne.
 * @return           Le nœud d'étagère correspondant, ou NULL s'il n'est pas trouvé.
 */
struct Node *getNodeShelf(struct Graph *G, int department, int shelf);



/**
 * Retourne un pointeur vers un nœud intermédiaire dans un graphe.
 *
 * @param G               Le graphe dans lequel rechercher le nœud intermédiaire.
 * @param department      Le numéro du rayon du nœud intermédiaire recherché.
 * @param boolTopBottom   Le booléen indiquant si le nœud intermédiaire est du haut (1) ou du bas (2).
 * @return                Un pointeur vers le nœud intermédiaire correspondant, ou NULL si aucun nœud n'a été trouvé.
 */
struct Node *getNodeIntermediate(struct Graph *G, int department, int boolTopBottom);

/**
 * Crée un lien entre deux nœuds dans un graphe.
 *
 * @param node1    Le premier nœud à relier.
 * @param node2    Le deuxième nœud à relier.
 * @param print    Un entier indiquant si l'impression est activée (1) ou désactivée (0).
 */
void createLink(struct Node *node1, struct Node *node2, int print);


/**
 * Crée le graphe représentant le plan de stockage à partir d'un fichier.
 *
 * @param filename Le nom du fichier contenant les informations du plan de stockage.
 * @param print    Indique si les étapes de création du graphe doivent être affichées (1) ou non (0).
 * @return         Le graphe créé.
 */
void *FileRead(struct Graph *G, char *filename, int print);

/**
 * Calcule la distance euclidienne entre deux nœuds dans un graphe.
 *
 * @param graph     Le graphe dans lequel les nœuds sont situés.
 * @param nd1       L'indice du premier nœud.
 * @param nd2       L'indice du deuxième nœud.
 *
 * @return          La distance euclidienne entre les deux nœuds arrondie à l'entier le plus proche.
 */
int distance(struct Graph *graph, int nd1, int nd2);

/**
 * Effectue l'algorithme de Bellman-Ford sur un graphe à partir d'un nœud source.
 * Calcule les distances minimales entre le nœud source et tous les autres nœuds du graphe.
 *
 * @param graph         Le graphe sur lequel l'algorithme est appliqué.
 * @param sourceNode    L'indice du nœud source à partir duquel calculer les distances.
 * @param tab           Tableau de structures eltBellman pour stocker les distances et les prédécesseurs.
 */
void algoBellman(struct Graph *graph, int sourceNode, struct eltBellman tab[]);

/**
 * Affiche le chemin le plus court passant par le panier dans un graphe, ainsi que les articles correspondants.
 *
 * @param graph         Le graphe dans lequel le chemin est calculé.
 * @param path          Tableau contenant le chemin le plus court.
 * @param panier        Tableau contenant les indices des articles dans le panier.
 * @param catalogue     Tableau des articles avec leurs références.
 */
void print_path(struct Graph *graph, int path[], int panier[], struct Article catalogue[], int print);


/**
 * Remplit le catalogue d'articles à partir d'un fichier.
 *
 * @param nom_fichier    Le nom du fichier contenant les informations des articles.
 * @return               Un pointeur vers le tableau d'articles rempli.
 */
Article* remplir_catalogue(char* nom_fichier);

/**
 * Affiche le contenu du catalogue d'articles.
 *
 * @param catalogue    Le tableau d'articles à afficher.
 */
void print_catalogue(Article* catalogue, int print);

/**
 * Dessine un carré centré aux coordonnées (x, y) avec une taille donnée.
 *
 * @param x     Coordonnée x du centre du carré.
 * @param y     Coordonnée y du centre du carré.
 * @param size  Taille du carré.
 * @param bool_panier   Indique si le carré est un panier (1) ou non (0).
 */
void drawSquare(int x, int y, int size, int bool_panier);

/**
 * Dessine une ligne entre les coordonnées (x1, y1) et (x2, y2).
 *
 * @param x1    Coordonnée x du premier point de la ligne.
 * @param y1    Coordonnée y du premier point de la ligne.
 * @param x2    Coordonnée x du deuxième point de la ligne.
 * @param y2    Coordonnée y du deuxième point de la ligne.
 */
void drawLine(int x1, int y1, int x2, int y2);

/**
 * Fonction d'affichage principale.
 * Cette fonction est appelée à chaque rafraîchissement de l'écran.
 * Elle dessine les éléments du magasin en utilisant OpenGL.
 */
void display();

/**
 * Fonction d'initialisation d'OpenGL.
 * Cette fonction est appelée au démarrage du programme pour configurer les paramètres d'OpenGL.
 */
void init();

/**
 * Fonction de redimensionnement de la fenêtre OpenGL.
 *
 * @param w     Nouvelle largeur de la fenêtre.
 * @param h     Nouvelle hauteur de la fenêtre.
 */
void reshape(int w, int h);

/**
 * Fonction principale du programme OpenGL.
 * Elle initialise GLUT, crée la fenêtre et lance la boucle principale.
 *
 * @return      Code de retour du programme.
 */
int main_openGL();


#endif // __GRAPH_H__
