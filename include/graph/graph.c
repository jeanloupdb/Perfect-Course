#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "graph.h"

  
/*struct PlanStock
{ 
	int nbRangees;
	int nbCasiers;
	int produit[][];
};*/



struct Graph G;



void printNode(struct Node *node)
{
	if (node->boolShelf==0)
	{
		printf("[%d,", node->aisle);
		if (node->boolTopBottom == 1)
			printf("top");
		else 
			printf("bottom");
		printf("]");
	}

	if (node->boolShelf==1)
		printf("[%d,%d]", node->aisle, node->shelf);

	if (node->boolShelf==2)
		printf("debut");
		
}

struct Node *createNode(int boolShelf, int aisle, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3) 
{
	struct Node *node;
	node=malloc(sizeof(struct Node));

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
	
	printf("create node: ");
	printNode(node);
	printf("(%d, %d) nb products: %d \n", node->X, node->Y, node->nbProducts);
	return node;
}

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


void createLink(struct Node *node1, struct Node *node2) 
{
	if ((node1->nbAdj>=10)||(node2->nbAdj>=10))
		errx(1,"link creation error");
	node1->adjTab[node1->nbAdj]=node2;
	node1->nbAdj++;
	node2->adjTab[node2->nbAdj]=node1;
	node2->nbAdj++;

	printf("create link: (");
	printNode(node1);

	printf(" - ");
	printNode(node2);
	
	printf(")\n");
}

struct Graph *FileRead(char *filename)
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
	
	while((fgets(buffer,256,file) != NULL) && 
			(sscanf(buffer,"aisle %d top(%d,%d) bottom(%d,%d", &aisle, &X1, &Y1, &X2, &Y2) == 5))
	{

		createNode(0,aisle,-1,1,X1,Y1,-1,-1,-1); /*noeud top*/
		while((fgets(buffer,256,file) != NULL ) && 
				(sscanf(buffer, "%d(%d,%d) %d %d %d", &shelf, &X1, &Y1,&article_1, &article_2, &article_3) == 6))
		{
			/*printf("shelf: %d a1: %d a2: %d a3: %d \n",shelf,article_1,article_2,article_3);*/
			createNode(1,aisle,shelf,-1,X1,Y1,article_1,article_2,article_3); /*noeud shelf*/
			if ((shelf == 1) || (shelf == 2))
				createLink(getNodeShelf(aisle,shelf),getNodeIntermediate(aisle,1));
			else
				createLink(getNodeShelf(aisle,shelf),getNodeShelf(aisle,shelf-2));
		}
		/*noeud bottom du dernier rayon*/
		createNode(0,aisle,-1,0,X2,Y2,-1,-1,-1);
		createLink(getNodeIntermediate(aisle,0),getNodeShelf(aisle,shelf));
		createLink(getNodeIntermediate(aisle,0),getNodeShelf(aisle,shelf-1));
	}

	while((fgets(buffer,256,file) != NULL) && (sscanf(buffer,"%d_%s %d_%s", &aisle_1, botop_1, &aisle_2, botop_2) == 4))
	{	
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,1));
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,0),getNodeIntermediate(aisle_2,1));
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,0));
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(aisle_1,0),getNodeIntermediate(aisle_2,0));
		/*printf("%d%s - %d%s \n", aisle_1, botop_1, aisle_2, botop_2);*/
		/* lien entre noeud top/bottom du rayon 1 et le noeud top/bottom du rayon 2*/
	}

	if (sscanf(buffer, "debut(%d,%d) %d_%s", &X1, &Y1, &aisle_1, botop_1) != 4)
		errx(1, "scanning error 5");
	/*printf("debut - %d_%s \n", aisle_1, botop_1);*/

	debut = createNode(2,-1,-1,-1, X1, Y1, -1, -1, -1);
	if (strcmp(botop_1,"top") == 0)
		createLink(debut,getNodeIntermediate(aisle_1,1));
	
	if (strcmp(botop_1,"bottom") == 0)
		createLink(debut,getNodeIntermediate(aisle_1,0));
	
	/* créer le noeud debut et le relier au noeud top/bottom du rayon*/
	fclose(file);
	return &G;
}

void freeGraph(struct Graph *graph) {
    // Libération de la mémoire allouée pour chaque noeud
    for(int i = 0; i < graph->order; i++) {
        free(graph->tableNodes[i]);
    }
    // Libération de la mémoire allouée pour le tableau de noeuds
    free(graph->tableNodes);
    // Libération de la mémoire allouée pour le graphe
    free(graph);
}




// catalogue

Article* remplir_catalogue(char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(1);
    }

    Article* articles = malloc(69 * sizeof(Article));
    if (articles == NULL) {
        printf("Impossible d'allouer de la mémoire\n");
        exit(1);
    }

    char ligne[100];
    int i = 0;
	char *token;
	while (fgets(ligne, 100, fichier) != NULL) {
		token = strtok(ligne, " ");
		articles[i].ref = atoi(token);
		token = strtok(NULL, " ");
		articles[i].nom = malloc(strlen(token) + 1);
		strcpy(articles[i].nom, token);
		token = strtok(NULL, " ");
		articles[i].prix = atof(token);
		token = strtok(NULL, " ");
		articles[i].poids = atof(token);
		token = strtok(NULL, " ");
		articles[i].volume = atof(token);
		token = strtok(NULL, " ");
		articles[i].resistance = atof(token);
		i++;
	}
    fclose(fichier);

    return articles;
}

void print_catalogue(Article* articles) {
	printf("\n\nCatalogue :\n\n");
    printf("+------+--------------------------+------------+--------+--------+------------+\n");
    printf("| Ref. | Nom                      |   Prix     |  Poids | Volume | Resistance |\n");
    printf("+------+--------------------------+------------+--------+--------+------------+\n");
    for (int i = 0; i < 69; i++) {
        printf("| %4d | %-24s | %10.2f | %6.2f | %6.2f | %10.2f |\n",
               articles[i].ref, articles[i].nom, articles[i].prix, articles[i].poids, articles[i].volume, articles[i].resistance);
    }
    printf("+------+--------------------------+------------+--------+--------+------------+\n");
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
	SHOP = malloc(sizeof(struct Graph));
	SHOP = FileRead("shop1.txt");
	// catalogue
	Article* articles = remplir_catalogue("catalogue.txt");
	print_catalogue(articles);


	
	
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

	// Dessiner les carrés pour les articles
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

int main(int argc, char **argv) {
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








