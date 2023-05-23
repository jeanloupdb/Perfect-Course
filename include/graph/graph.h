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
    int aisle; 
    int shelf; 
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
    float poids;       // Poids de l'article
    float volume;      // Volume de l'article
    float resistance;  // Résistance de l'article
} Article;


struct eltBellman 
{
        int distance;
        int precedentNode;
};


void printNode(struct Node *node);
struct Node *createNode(int boolShelf, int aisle, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3, int print);
struct Node *getNodeShelf(int aisle, int shelf);
struct Node *getNodeIntermediate(int aisle, int boolTopBottom);
void createLink(struct Node *node1, struct Node *node2, int print);
struct Graph *FileRead(char *filename, int print);
Article* remplir_catalogue(char* nom_fichier) ;
int ref_to_node(struct Graph *G, int ref);
void print_catalogue(Article* catalogue);
int main_openGL() ;
void reshape(int w, int h);
void init();

#endif // __GRAPH_H__
