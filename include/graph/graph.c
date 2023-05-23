#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "graph.h"



struct Graph G;

void printNode(struct Node *node)
{
	//boolShelf = 0 -> noeud extremite d'une etagere
	//    boolTopBottom = 1 -> noeud du haut
	//    sinon -> noeud du bas
	//boolShelf = 1 -> noeud produit
	//boolShelf = 2 -> noeud entree de l'entrepot

	char *top = "top\0";
	char *bottom = "bottom\0";
	char *product = "product\0";
	char *entry = "entry\0";
	char *s;
	if(node->boolShelf==0){
		if(node->boolTopBottom==1)
			s = top;
		else
			s = bottom;
	}
	else if(node->boolShelf==1)
		s = product;
	else
		s = entry;

	//|    Action   | NodeID  |  Type   | Rayon | Etagere | X,Y   | NbProducts |
	printf("\n| create node | %6d  | %7s | %5d | %7d | %2d,%2d | %10d |", node->nodeID, s, node->aisle, node->shelf, node->X, node->Y, node->nbProducts);
}

void printLink(struct Node *node1, struct Node *node2)
{
	printf(" (%d to %d)", node1->nodeID, node2->nodeID);
}

// createNode cree un noeud
struct Node *createNode(int boolShelf, int aisle, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3,  int print) 
{
	struct Node *node;
	node=malloc(sizeof(struct Node));
	node->nodeID=G.order;
	node->boolShelf=boolShelf;
	node->aisle=aisle;
	node->shelf=shelf;
	node->boolTopBottom=boolTopBottom;
	node->nbAdj=0;
	node->nbProducts=0;

	node->X = X;
	node->Y = Y;

	node->productsTab[0]=p1;
	node->productsTab[1]=p2;
	node->productsTab[2]=p3;
	if (p1 != -1)
		node->nbProducts++;
	if (p2 != -1)
		node->nbProducts++;
	if (p3 != -1)
		node->nbProducts++;

	G.tableNodes[G.order]=node;
	G.order++;
	
	if (print == 1){
		printNode(node);
	}
	return node;
}

// getNodeShelf(int aisle, int shelf)  retourne le noeud shelf correspondant a l'etage et a la colonne
struct Node *getNodeShelf(int aisle, int shelf) 
{
	int i=0;
	while (i<G.order) 
	{
		if ((aisle==G.tableNodes[i]->aisle) && (shelf==G.tableNodes[i]->shelf))
			return G.tableNodes[i];
		i++;
	}
	return NULL;
}


// getNodeIntermediate retourne le noeud intermediaire entre deux noeuds shelf
struct Node *getNodeIntermediate(int aisle, int boolTopBottom)
{
	int i=0;
	while (i<G.order) {
		if ((aisle==G.tableNodes[i]->aisle) && (boolTopBottom==G.tableNodes[i]->boolTopBottom))
			return G.tableNodes[i];
		i++;
		}
	return NULL;
}

// createLink cree un lien entre deux noeuds
void createLink(struct Node *node1, struct Node *node2,  int print) 
{
	if ((node1->nbAdj>=10)||(node2->nbAdj>=10))
		errx(1,"link creation error");

	int ok = 1;
	int i=0;
	while ((i<node1->nbAdj) && (ok==1)) {
		if (node1->adjTab[i]->nodeID==node2->nodeID)
			ok=0;
		i++;
	}
	if(ok==1){
		i=0;
		while ((i<node2->nbAdj) && (ok==1)) {
			if (node2->adjTab[i]->nodeID==node1->nodeID)
				ok=0;
			i++;
		}
	}
	if(ok==1){
		node1->adjTab[node1->nbAdj]=node2;
		node1->nbAdj++;
		node2->adjTab[node2->nbAdj]=node1;
		node2->nbAdj++;

		if (print == 1)
		{
			printLink(node1,node2);
		}
	}
}

// Creation du graphe representant le plan de stockage a partir d'un fichier
// print = 1 pour afficher les etapes de creation du graphe
struct Graph *FileRead(char *filename , int print)
{
	FILE *file;
	char buffer[256];
	int aisle;int shelf;
	int article_1;int article_2;int article_3;
	int aisle_1;int aisle_2;
	char botop_1[50];char botop_2[50];
	struct Node *debut;
	int X1; int Y1; int X2; int Y2;
	

	file = fopen(filename, "r");
	if (file == NULL)
		errx(1,"opening error");

	if(print == 1){
		printf("+-------------+---------+---------+-------+---------+-------+------------+\n");
		printf("|    Action   | NodeID  |  Type   | Rayon | Etagere | X,Y   | NbProducts |\n");
		printf("|-------------|---------|---------|-------|---------|-------|------------|");        
	}     
	while((fgets(buffer,256,file) != NULL) && 
			(sscanf(buffer,"aisle %d top(%d,%d) bottom(%d,%d", &aisle, &X1, &Y1, &X2, &Y2) == 5))
	{

		createNode(0,aisle,-1,1,X1,Y1,-1,-1,-1,  print); /*noeud top*/   
		while((fgets(buffer,256,file) != NULL ) && 
				(sscanf(buffer, "%d(%d,%d) %d %d %d", &shelf, &X1, &Y1,&article_1, &article_2, &article_3) == 6))
		{
			/*printf("shelf: %d a1: %d a2: %d a3: %d \n",shelf,article_1,article_2,article_3);*/
			createNode(1,aisle,shelf,-1,X1,Y1,article_1,article_2,article_3, print); /*noeud shelf*/  
			if ((shelf == 1) || (shelf == 2))
				createLink(getNodeShelf(aisle,shelf),getNodeIntermediate(aisle,1),   print);   
			else
				createLink(getNodeShelf(aisle,shelf),getNodeShelf(aisle,shelf-2),   print);   
		}
		/*noeud bottom du dernier rayon*/
		createNode(0,aisle,-1,0,X2,Y2,-1,-1,-1,   print);   
		createLink(getNodeIntermediate(aisle,0),getNodeShelf(aisle,shelf)  , print);   
		createLink(getNodeIntermediate(aisle,0),getNodeShelf(aisle,shelf-1) , print);   
	}


	
	//debut
	while((fgets(buffer,256,file) != NULL) && (sscanf(buffer,"start(%d,%d)", &X1, &Y1) == 2))
	{
		debut=createNode(2,-1,-1,-1,X1,Y1,-1,-1,-1,print);   
	}
	 


	while((fgets(buffer,256,file) != NULL) && ((sscanf(buffer,"%d_%s %d_%s", &aisle_1, botop_1, &aisle_2, botop_2) == 4) 
			|| (sscanf(buffer,"%s %d_%s", botop_1, &aisle_2, botop_2) == 3)))
	{	
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,1) ,   print);   
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,0),getNodeIntermediate(aisle_2,1)  ,   print);   
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,0)  ,   print);   
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(aisle_1,0),getNodeIntermediate(aisle_2,0)  ,   print);
		if(strcmp(botop_1,"start") == 0 && strcmp(botop_2,"top") == 0)
			createLink(debut,getNodeIntermediate(aisle_2,1)  ,   print);   /* lien entre le noeud start et le noeud top du rayon 2*/
		if(strcmp(botop_1,"start") == 0 && strcmp(botop_2,"bottom") == 0)
			createLink(debut,getNodeIntermediate(aisle_2,0)  ,   print);   /* lien entre le noeud start et le noeud bottom du rayon 2*/
	}

	
	fclose(file);

	if(print == 1)
		printf("\n+-------------+---------+---------+-------+---------+-------+------------+\n");

	return &G;
}








// poids entre deux noeuds selon la distance euclidienne
int poids(struct Graph *graph, int nd1, int nd2) 
{
	int x1, y1, x2, y2;
	x1=graph->tableNodes[nd1]->X;
	y1=graph->tableNodes[nd1]->Y;
	x2=graph->tableNodes[nd2]->X;
	y2=graph->tableNodes[nd2]->Y;

	return (int) sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}


// Algorithme de Bellman retourne le tableau des distances et des prédécesseurs
void algoBellman(struct Graph *graph, int sourceNode, struct eltBellman tab[]) 
{
	int i=0;
	while (i<graph->order)
	{
		if (i == sourceNode)
		{
			tab[i].distance=0;
			tab[i].precedentNode=-1;
		}
		else
		{
			tab[i].distance=9999;
			tab[i].precedentNode=-1;
		}
		i++;
	}

	i=1;
	while (i<graph->order)
	{
		int N=0;
		while (N<graph->order)
		{
		    if (N != sourceNode)
			{
				int rgAdj=0;
				while (rgAdj<graph->tableNodes[N]->nbAdj)
				{
					int P = graph->tableNodes[N]->adjTab[rgAdj]->nodeID;
					int poidsArc= (int)poids(graph,N,P);
					if (tab[N].distance > tab[P].distance+poidsArc)
					{
						tab[N].distance=tab[P].distance+poidsArc;
						tab[N].precedentNode=P;
					}
					rgAdj++;
				}

			}
			N++;	    
		}
		i++;
	}
}

// fonction print bellman
void printBellman(struct eltBellman tab[] , int order)
{
	int i=0;
    while (i<order) 
    {
        printf("nodeID: %d, dist: %d, prec: %d\n",i,tab[i].distance,tab[i].precedentNode);
        i++;
	}

}

void pathToTarget(struct eltBellman tabBellman[], int sourceNode, int targetNode, int pathToTarget[], int *nbPathNodes)
{
	int node=targetNode;
	int i=0;
	int pathToTargetInverse[512];
	*nbPathNodes=0;

	while (tabBellman[node].precedentNode!=-1)
		{
			pathToTargetInverse[i]=node;
			i++;
			*nbPathNodes=*nbPathNodes+1;
			node=tabBellman[node].precedentNode;
		}
	if (node!=sourceNode) 
		printf("Error sourceNode in pathToNode\n");
	else
	{
		pathToTargetInverse[i]=node;
		*nbPathNodes=*nbPathNodes+1;

		/* Inversion du path */
		i=0;
		while (i<*nbPathNodes)
			{
				int node=*nbPathNodes-1-i;
				pathToTarget[node]=pathToTargetInverse[i];
				i++;
			}
	}
}

// void printPathToTarget(struct Graph *graph, int pathToTarget[], int nbPathNodes)
// {
// 	int i=0;
// 	printf("path\n");
// 	while (i<nbPathNodes)
// 		{
// 			int node=pathToTarget[i];
// 			printf("->Id%d[%d,%d](%d,%d,%d)",node,graph->tableNodes[node]->aisle,graph->tableNodes[node]->shelf,
// 				graph->tableNodes[node]->productsTab[0],
// 				graph->tableNodes[node]->productsTab[1],
// 				graph->tableNodes[node]->productsTab[2]);

// 			i++;
// 		}
// 	printf("\n");
// }

// different path to target 2

void printPathToTarget(struct Graph *graph, int pathToTarget[], int nbPathNodes, int sourceNode, int targetNode)
{
    printf("\n\nPlus court chemin : %d => %d\n", sourceNode, targetNode);
	for(int i = 0; i < nbPathNodes; i++) {
		int node = pathToTarget[i];
		printf("-> %d ", node);
	}

}

void print_panier(struct Graph *graph, int panier[], Article catalogue[])
{
    printf("\n\nPrint panier :\n");
    printf("+------+----------------------------+-----------+-------------+\n");
    printf("| %-4s | %-26s | %-10s | %-11s |\n", "ref", "Article", "Quantité", "N° de noeud");
    printf("+------+----------------------------+-----------+-------------+\n");

    for(int i = 0; i < 69; i++) {
        if(panier[i] != -1)
            printf("| %-4d | %-26s | %-9d | %-11d |\n",catalogue[i].ref, catalogue[i].nom, panier[i], ref_to_node(graph, catalogue[i].ref));
		
    }


    printf("+------+----------------------------+-----------+-------------+\n");
}

// transforme la ref en nodes
int ref_to_node(struct Graph *G, int ref)
{
	int res = -1;
	for(int i = 0; i < G->order; i++) {
		if(G->tableNodes[i]->productsTab[0] == ref || G->tableNodes[i]->productsTab[1] == ref || G->tableNodes[i]->productsTab[2] == ref)
			res = i;
	}
	return res;
}

// fonction qui trouve le plus court chemin dans les noueds du panier
// panier : int [69] avec panier[i] > 0 si l'article i est dans le panier
// renvoie le chemin sous forme d'un tableau de nodes
//plus_court_chemin(G, panier, catalogue, sourceNode, tab, path);

void plus_court_chemin(struct Graph *graph, int panier[], Article catalogue[], int sourceNode, struct eltBellman tab[], int path2[], int nbart)
{
	int nbPathNodes;
	int targetNode;
	int nbNodes = 0;
	int nodes[nbart];
	int path[512];
	int countPath = 0;
	// on récupère les nodes du panier
	for(int i = 0; i < 69; i++) {
		if(panier[i] != -1) {
			nodes[nbNodes] = ref_to_node(graph, i);
			nbNodes++;
		}
	}

	int bool = 0;
	while(!bool){
		algoBellman(graph, sourceNode, tab);
		int test = 0;
		for (int k = 0; k < nbart; k++)
		{
			test += (nodes[k] != -1);
		}
		bool = test == 0;
		// on trouve le noeud de nodes[] le plus proche de sourceNode
		int min = 100000;
		
		int i = 0;
		int i_min = 0;
		while(i < nbart) {
			if(tab[nodes[i]].distance < min && nodes[i] != -1) {
				min = tab[nodes[i]].distance;
				targetNode = nodes[i];
				i_min = i;
			}
			i++;
		}
		nodes[i_min] = -1;

		// on trouve le chemin
		pathToTarget(tab, sourceNode, targetNode, path, &nbPathNodes);
		// on affiche le chemin
		printPathToTarget(graph, path, nbPathNodes, sourceNode, targetNode);
		for(int i = 0; i < nbPathNodes; i++) {
			path2[countPath] = path[i];
			countPath++;
		}
		sourceNode = targetNode;
	}
	
}

void print_path(struct Graph *graph, int path[], int panier[], struct Article catalogue[])
{
	int inter = 0;
	printf("\n\n -------------------------------------------------------------------------------------------\n");
	printf(" ---------------------  Plus court chemin passant par le panier :  -------------------------\n");
	printf(" -------------------------------------------------------------------------------------------\n\n");

	printf("+-------------+----------------------------+------+\n");
	printf("| %-11s | %-26s | %-4s |\n", "N° de noeud", "Article", "ref");
	printf("+-------------+----------------------------+------+\n");
	int test = 0;
	for (int i = 0; i < 512; i++)
	{
		if(path[i] != -1 && path[i] != inter)
		{
			printf("| %-11d |", path[i]);
		
			for (int j = 0; j < 69; j++)
			{

				if(panier[j] != -1 && path[i] == ref_to_node(graph, j))
				{
					printf(" %-26s | %-4d |", catalogue[j].nom, j);
					test = 1;
				}

			}

			inter = path[i];
			if(test == 0)
				printf(" %-26s | %-4s |", " ", " ");
			printf("\n");
			test = 0;
		}
			
	}
	printf("+-------------+----------------------------+------+\n");
}


int main(int argc, char argv[])
{
	struct Graph *G;
    struct eltBellman tab[512];
    int i=0;
    int path[512];

	// initialisation du tableau path à -1
	for(int i = 0; i < 512; i++) {
		path[i] = -1;
	}

    int nbPathNodes;

    int sourceNode;
    int targetNode;

    // if (argc != 2)
    //     errx(1,"invalid arg");
	printf("\n\nFileRead(\"shop1.txt\") :\n");
    G = FileRead("shop1.txt", 1);

    // // sourceNode=G->order-1; /* debut */
	// sourceNode=45;
    // targetNode=55;

    algoBellman(G, 38, tab);

	printf("\n\nprint bellman(38)\n");
    // while (i<G->order) 
    // {
    //     printf("node%d: distance=%d, precedent:%d\n",i,tab[i].distance,tab[i].precedentNode);
    //     i++;
    // }
	printBellman(tab, G->order);

    pathToTarget(tab, 38, 12, path, &nbPathNodes);
    printPathToTarget(G, path, nbPathNodes, 38, 12);

	Article* catalogue = remplir_catalogue("catalogue.txt");
	print_catalogue(catalogue);


	// panier int panier[69];
	int panier[69];
	for (int i = 0; i < 69; i++) {
		panier[i] = -1;
	}
	panier[0] = 1; //  1 lait
	panier[5] = 1; //  1 pain
	panier[12] = 1; //  1 the_en_sachets
	panier[25] = 1; //  1 haricots_blancs
	panier[45] = 1;	//  1 oeufs_de_caille
	panier[55] = 1; //  1 raisin
	panier[61] = 1; //  1 carotte 
	panier[64] = 3; //  1 oignon
	panier[68] = 1; //  1 patate_douce
	int nbart = 9;

	int panierCopy[69];
	for (int i = 0; i < 69; i++) {
		panierCopy[i] = panier[i];
	}

	print_panier(G, panierCopy, catalogue);

	
	plus_court_chemin(G, panierCopy, catalogue, 38, tab, path, nbart);

	print_path(G, path, panier, catalogue);


	//Interface graphique
	main_openGL();

    return 0;
}


// catalogue

Article* remplir_catalogue(char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(1);
    }

    Article* catalogue = malloc(69 * sizeof(Article));
    if (catalogue == NULL) {
        printf("Impossible d'allouer de la mémoire\n");
        exit(1);
    }

    char ligne[100];
    int i = 0;
	char *token;
	while (fgets(ligne, 100, fichier) != NULL) {
		token = strtok(ligne, " ");
		catalogue[i].ref = atoi(token);
		token = strtok(NULL, " ");
		catalogue[i].nom = malloc(strlen(token) + 1);
		strcpy(catalogue[i].nom, token);
		token = strtok(NULL, " ");
		catalogue[i].prix = atof(token);
		token = strtok(NULL, " ");
		catalogue[i].poids = atof(token);
		token = strtok(NULL, " ");
		catalogue[i].volume = atof(token);
		token = strtok(NULL, " ");
		catalogue[i].resistance = atof(token);
		i++;
	}
    fclose(fichier);

    return catalogue;
}

void print_catalogue(Article* catalogue) {
	printf("\n\nCatalogue :\n\n");
    printf("+------+----------------------------+------------+--------+--------+------------+\n");
    printf("| Ref. | Nom                        |   Prix     |  Poids | Volume | Resistance |\n");
    printf("+------+----------------------------+------------+--------+--------+------------+\n");
    for (int i = 0; i < 69; i++) {
        printf("| %4d | %-26s | %10.2f | %6.2f | %6.2f | %10.2f |\n",
               catalogue[i].ref, catalogue[i].nom, catalogue[i].prix, catalogue[i].poids, catalogue[i].volume, catalogue[i].resistance);
    }
    printf("+------+----------------------------+------------+--------+--------+------------+\n");
}









//representation graphique :

 
//                                                                               mm               mm      db                                                                   `7MM          db                                  
//                                                                               MM               MM                                                                             MM                                              
// `7Mb,od8  .gP"Ya  `7MMpdMAo. `7Mb,od8  .gP"Ya  ,pP"Ybd  .gP"Ya  `7MMpMMMb.  mmMMmm   ,6"Yb.  mmMMmm  `7MM   ,pW"Wq.  `7MMpMMMb.       .P"Ybmmm `7Mb,od8  ,6"Yb.  `7MMpdMAo.   MMpMMMb.  `7MM    ,dW"Yvd  `7MM  `7MM   .gP"Ya  
//   MM' "' ,M'   Yb   MM   `Wb   MM' "' ,M'   Yb 8I   `" ,M'   Yb   MM    MM    MM    8)   MM    MM      MM  6W'   `Wb   MM    MM      :MI  I8     MM' "' 8)   MM    MM   `Wb   MM    MM    MM   ,W'   MM    MM    MM  ,M'   Yb 
//   MM     8M""""""   MM    M8   MM     8M"""""" `YMMMa. 8M""""""   MM    MM    MM     ,pm9MM    MM      MM  8M     M8   MM    MM       WmmmP"     MM      ,pm9MM    MM    M8   MM    MM    MM   8M    MM    MM    MM  8M"""""" 
//   MM     YM.    ,   MM   ,AP   MM     YM.    , L.   I8 YM.    ,   MM    MM    MM    8M   MM    MM      MM  YA.   ,A9   MM    MM      8M          MM     8M   MM    MM   ,AP   MM    MM    MM   YA.   MM    MM    MM  YM.    , 
// .JMML.    `Mbmmd'   MMbmmd'  .JMML.    `Mbmmd' M9mmmP'  `Mbmmd' .JMML  JMML.  `Mbmo `Moo9^Yo.  `Mbmo .JMML. `Ybmd9'  .JMML  JMML.     YMMMMMb  .JMML.   `Moo9^Yo.  MMbmmd'  .JMML  JMML..JMML.  `MbmdMM    `Mbod"YML. `Mbmmd' 
//                     MM                                                                                                               6'     dP                     MM                                MM                       
//                   .JMML.                                                                                                             Ybmmmd'                     .JMML.                            .JMML.        



// Fonction de dessin des carrés
void drawSquare(int x, int y, int size) {
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f); // Bleu
	glVertex2i(x - size, y - size);
	glVertex2i(x + size, y - size);
	glVertex2i(x + size, y + size);
	glVertex2i(x - size, y + size);
	glEnd();
}

// Fonction de dessin des traits
void drawLine(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
}

// Fonction d'affichage
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	// Creer le Magasin :
	struct Graph *SHOP;
	SHOP = FileRead("shop1.txt", 0);
	
	// Dessiner les traits pour les liens
	glColor3f(1.0, 0.0, 0.0); // Rouge
	for (int i = 0; i < SHOP->order; i++) {
		struct Node *node = SHOP->tableNodes[i];
		for (int j = 0; j < node->nbAdj; j++) {
			struct Node *adjNode = node->adjTab[j];
			double distance = sqrt(pow(adjNode->X - node->X, 2) + pow(adjNode->Y - node->Y, 2));
			if (distance >= 2.5) { // Si lien entre rayons
				
				// Modifier l'épaisseur de la ligne
        			glLineWidth(1);
				glColor3f(0.0, 1.0, 0.0); // Vert pour les liens entre rayons
			}else{
				// Modifier l'épaisseur de la ligne
        			glLineWidth(5);
				glColor3f(1.0, 0.0, 0.0); // Rouge pour les liens entre article
			}
			drawLine(node->X * 50, node->Y * 40, adjNode->X * 50, adjNode->Y * 40);
		}
	}

	// Dessiner les carrés pour les catalogue
	glColor3f(0.0, 0.0, 1.0); // Bleu
	int size = 7;
	for (int i = 0; i < SHOP->order; i++) {
		struct Node *node = SHOP->tableNodes[i];
		int x = node->X * 50;
		int y = node->Y * 40;
		drawSquare(x, y, size);
	}


	// Rafraîchir l'affichage
	glutSwapBuffers();
}

	// Fonction d'initialisation
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0); // Blanc
}

	// Fonction de redimensionnement de la fenêtre
void reshape(int w, int h) {
	// origine du repere 
	int x_origin = 100;
	int y_origin = 100;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_ratio = (float) w / h;
	gluOrtho2D(-x_origin, 800, -y_origin, 600);
	glMatrixMode(GL_MODELVIEW);
}

int main_openGL() {
	// Initialisation de GLUT, cration des arguments de gluinit
	int argc = 1;
	char *argv[1] = { (char*)"glut" };
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Magasin");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}








