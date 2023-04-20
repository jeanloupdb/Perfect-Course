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

struct Node
{
    int nodeID;
    int boolShelf;
    int aisle;
    int shelf;
    int boolTopBottom;
    struct Node *adjTab[10];
    int nbAdj;
    int X;
    int Y;
    int productsTab[3];
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
