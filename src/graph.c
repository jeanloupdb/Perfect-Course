#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "graph.h"



struct Graph *createGraph(int print)
{
	struct Graph *G;
	G=malloc(sizeof(struct Graph));
	G->order=0;
	if (print == 1)
		printf("\n| create graph |");
	return G;
}


void printNode(struct Node *node, int print)
{
	if(print){
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
		printf("\n| create node | %6d  | %7s | %5d | %7d | %2d,%2d | %10d |", node->nodeID, s, node->department, node->shelf, node->X, node->Y, node->nbProducts);
	}
}

void printLink(struct Node *node1, struct Node *node2, int print)
{
	if(print)
		printf(" (%d to %d)", node1->nodeID, node2->nodeID);
}

int getNbArticles(char *filename)
{
	FILE *file;
	char buffer[256];
	int nbArticles = 0;

	file = fopen(filename, "r");
	if (file == NULL)
		errx(1,"opening error");

	while (fgets(buffer, 256, file) != NULL)
	{
		nbArticles++;
	}
	fclose(file);
	return nbArticles;
}


struct Node *createNode(struct Graph *G , int boolShelf, int department, int shelf, int boolTopBottom, int X, int Y, int p1, int p2, int p3,  int print) 
{
	struct Node *node;
	node=malloc(sizeof(struct Node));
	node->nodeID=G->order;
	node->boolShelf=boolShelf;
	node->department=department;
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

	G->tableNodes[G->order]=node;
	G->order++;
	
	if (print == 1){
		printNode(node, print);
	}
	return node;
}

struct Node *getNodeShelf(struct Graph *G, int department, int shelf) 
{
	int i=0;
	while (i<G->order) 
	{
		if ((department==G->tableNodes[i]->department) && (shelf==G->tableNodes[i]->shelf))
			return G->tableNodes[i];
		i++;
	}
	return NULL;
}


struct Node *getNodeIntermediate(struct Graph *G, int department, int boolTopBottom)
{
	int i=0;
	while (i<G->order) {
		if ((department==G->tableNodes[i]->department) && (boolTopBottom==G->tableNodes[i]->boolTopBottom))
			return G->tableNodes[i];
		i++;
		}
	return NULL;
}

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
			printLink(node1,node2, print);
		}
	}
}


int *FileRead(struct Graph *G, char *filename , int print)
{
	FILE *file;
	char buffer[256];
	int department;int shelf;
	int article_1;int article_2;int article_3;
	int department_1;int department_2;
	char botop_1[50];char botop_2[50];
	struct Node *debut;
	int X1; int Y1; int X2; int Y2;
	int start = 0;
	

	file = fopen(filename, "r");
	if (file == NULL)
		errx(1,"opening error");

	if(print == 1){
		printf("+-------------+---------+---------+-------+---------+-------+------------+\n");
		printf("|    Action   | NodeID  |  Type   | Rayon | Etagere | X,Y   | NbProducts |\n");
		printf("|-------------|---------|---------|-------|---------|-------|------------|");        
	}     
	while((fgets(buffer,256,file) != NULL) && 
			(sscanf(buffer,"department %d top(%d,%d) bottom(%d,%d", &department, &X1, &Y1, &X2, &Y2) == 5))
	{
		createNode(G, 0,department,-1,1,X1,Y1,-1,-1,-1,  print); /*noeud top*/   
		while((fgets(buffer,256,file) != NULL ) && 
				(sscanf(buffer, "%d(%d,%d) %d %d %d", &shelf, &X1, &Y1,&article_1, &article_2, &article_3) == 6))
		{
			/*printf("shelf: %d a1: %d a2: %d a3: %d \n",shelf,article_1,article_2,article_3);*/
			createNode(G, 1,department,shelf,-1,X1,Y1,article_1,article_2,article_3, print); /*noeud shelf*/  
			if ((shelf == 1) || (shelf == 2))
				createLink(getNodeShelf(G, department,shelf),getNodeIntermediate(G, department,1),   print);   
			else
				createLink(getNodeShelf(G, department,shelf),getNodeShelf(G, department,shelf-2),   print);   
		}
		/*noeud bottom du dernier rayon*/
		createNode(G, 0,department,-1,0,X2,Y2,-1,-1,-1,   print);   
		createLink(getNodeIntermediate(G, department,0),getNodeShelf(G, department,shelf)  , print);   
		createLink(getNodeIntermediate(G, department,0),getNodeShelf(G, department,shelf-1) , print);   
	}

	//debut
	while((fgets(buffer,256,file) != NULL) && (sscanf(buffer,"start(%d,%d)", &X1, &Y1) == 2))
	{
		debut=createNode(G, 2,-1,-1,-1,X1,Y1,-1,-1,-1,print);
		// start = ref du noeud debut
		start = debut->nodeID;
	}
	 
	if(print == 1)
		printf("\n+-------------+---------+---------+-------+---------+-------+------------+");
	

	while((fgets(buffer,256,file) != NULL) && ((sscanf(buffer,"%d_%s %d_%s", &department_1, botop_1, &department_2, botop_2) == 4) 
			|| (sscanf(buffer,"%s %d_%s", botop_1, &department_2, botop_2) == 3)))
	{	
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(G, department_1,1),getNodeIntermediate(G, department_2,1) ,   print);   
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"top") == 0))
			createLink(getNodeIntermediate(G, department_1,0),getNodeIntermediate(G, department_2,1)  ,   print);   
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(G, department_1,1),getNodeIntermediate(G, department_2,0)  ,   print);   
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_2,"bottom") == 0))
			createLink(getNodeIntermediate(G, department_1,0),getNodeIntermediate(G, department_2,0)  ,   print);
		if(strcmp(botop_1,"start") == 0 && strcmp(botop_2,"top") == 0)
			createLink(debut,getNodeIntermediate(G, department_2,1)  ,   print);   /* lien entre le noeud start et le noeud top du rayon 2*/
		if(strcmp(botop_1,"start") == 0 && strcmp(botop_2,"bottom") == 0)
			createLink(debut,getNodeIntermediate(G, department_2,0)  ,   print);   /* lien entre le noeud start et le noeud bottom du rayon 2*/
	}
	fclose(file);
	return start;
}



int distance(struct Graph *graph, int nd1, int nd2) 
{
	int x1, y1, x2, y2;
	x1=graph->tableNodes[nd1]->X;
	y1=graph->tableNodes[nd1]->Y;
	x2=graph->tableNodes[nd2]->X;
	y2=graph->tableNodes[nd2]->Y;

	return (int) sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}


void algoBellman(struct Graph *graph, int sourceNode, struct eltBellman *tab) 
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
					int poidsArc= (int)distance(graph,N,P);
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
void printBellman(struct eltBellman *tab , int order, int print)
{
	if(print){
		int i=0;
		while (i<order) 
		{
			printf("nodeID: %d, dist: %d, prec: %d\n",i,tab[i].distance,tab[i].precedentNode);
			i++;
		}
	}

}

void pathToTarget(struct eltBellman *tabBellman, int sourceNode, int targetNode, int *pathToTarget, int *nbPathNodes)
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





void printPathToTarget(int *pathToTarget, int nbPathNodes, int sourceNode, int targetNode, int print)
{
	if(print){
		printf("\n\nPlus court chemin : %d => %d\n", sourceNode, targetNode);
		for(int i = 0; i < nbPathNodes; i++) {
			int node = pathToTarget[i];
			printf("-> %d ", node);
		}
	}

}
int getNbArticlesInPanier(int *panier, int nbArticles){
	int nbArticlesInPanier = 0;
	for(int i = 0; i < nbArticles; i++) {
		if(panier[i] != -1){
			nbArticlesInPanier++;
		}
	}
	return nbArticlesInPanier;
}

void print_panier(struct Graph *graph, int *panier, Article *catalogue, int nbArticles, int print)
{
	
	if(print){
    printf("\n\nPrint panier :\n");
    printf("+------+----------------------------+-----------+-------------+\n");
    printf("| %-4s | %-26s | %-10s | %-11s |\n", "ref", "Article", "Quantité", "N° de noeud");
    printf("+------+----------------------------+-----------+-------------+\n");
    for(int i = 0; i < nbArticles; i++) {
        if(panier[i] != -1){
    	    printf("| %-4d | %-26s | %-9d | %-11d |\n",catalogue[i].ref, catalogue[i].nom, panier[i], ref_to_node(graph, catalogue[i].ref));
		}
		
    }


    printf("+------+----------------------------+-----------+-------------+\n");
	}
}

int ref_to_node(struct Graph *G, int ref)
{
	int res = -1;
	for(int i = 0; i < G->order; i++) {
		if(G->tableNodes[i]->productsTab[0] == ref || G->tableNodes[i]->productsTab[1] == ref || G->tableNodes[i]->productsTab[2] == ref)
			res = i;
	}
	return res;
}

int scoreArticle(const void *a)
{
    const Article *articleA = (const Article *)a;

    // Facteurs de pondération pour les différents paramètres
    double poidsFactor = 0.5;
    double resistanceFactor = 1.0;
    double volumeFactor = 0.2;
	double frozFactor = 1.0;

	// Calcul du score
	double score = poidsFactor * articleA->poids + resistanceFactor * (1-articleA->resistance) + volumeFactor * articleA->volume + frozFactor * articleA->frozen;

	return score;
}




void remplir_panier(int* panier, char* file, int nbArticles) {
    for (int i = 0; i < nbArticles; i++) {
        panier[i] = -1;
    }

    FILE* fichier = fopen(file, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int ref, qte;
        if (sscanf(ligne, "%d %d", &ref, &qte) == 2) {
            if (ref >= 0 && ref < nbArticles) {
                panier[ref] = qte;
            } else {
                printf("Référence invalide : %d\n", ref);
            }
        }
    }

    fclose(fichier);
}




int *plus_court_chemin(struct Graph *graph, int *panier, Article *catalogue, int sourceNode, struct eltBellman *tab, int *path2, int NbArticlesInPanier, int nbArticles,  int print) {
    int nbPathNodes;
    int targetNode;
    int nbNodes = 0;
    // int nodes[NbArticlesInPanier];
	int *nodes = malloc(NbArticlesInPanier * sizeof(int));
    int path[512];
    int countPath = 0;

    // On récupère les nodes du panier
    for (int i = 0; i < nbArticles; i++) {
        if (panier[i] != -1) {
            nodes[nbNodes] = ref_to_node(graph, i);
            nbNodes++;
        }
    }

    // On trie les nodes du panier par ordre selon la fonction scoreArticle(const void *a)
    for (int i = 0; i < nbNodes; i++) {
        for (int j = i + 1; j < nbNodes; j++) {
            if (scoreArticle(&catalogue[graph->tableNodes[nodes[i]]->productsTab[0]]) < scoreArticle(&catalogue[graph->tableNodes[nodes[j]]->productsTab[0]])) {
                int temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;
            }
        }
    }

    // On applique l'algorithme de Bellman-Ford sur les noeuds triés
    for (int i = 0; i < nbNodes; i++) {
        algoBellman(graph, sourceNode, tab);
        targetNode = nodes[i];
        pathToTarget(tab, sourceNode, targetNode, path, &nbPathNodes);
        printPathToTarget(path, nbPathNodes, sourceNode, targetNode, print);

        for (int j = 0; j < nbPathNodes; j++) {
            path2[countPath] = path[j];
            countPath++;
        }

        sourceNode = targetNode;
    }
	return nodes;
}


void print_path(struct Graph *graph, int *path, int *panier, struct Article *catalogue, int nbArticles, int print)
{
	if(print)
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
		
			for (int j = 0; j < nbArticles; j++)
			{

				if(panier[j] != -1 && path[i] == ref_to_node(graph, j) && test == 0)
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
}



int *creer_chemin(struct Graph *G, int *path, int *panier, struct Article *catalogue, struct eltBellman *tab, int nbArticles, int print)
{

	// initialisation du tableau path à -1
	for(int i = 0; i < 512; i++) {
		path[i] = -1;
	}

    int nbPathNodes;


    // if (argc != 2)
    //     errx(1,"invalid arg");

	
	if(print)
		printf("\n\nFileRead(\"bdd/shop1.txt\") :\n");
    int start = FileRead(G, "bdd/shop1.txt", print);

    // // sourceNode=G->order-1; /* debut */
	// sourceNode=45;
    // targetNode=55;

    

	remplir_catalogue(catalogue, "bdd/catalogue.txt");

	print_catalogue(catalogue, nbArticles, print);

	remplir_panier(panier, "bdd/cart.txt", nbArticles);
	int NbArticlesInPanier = getNbArticlesInPanier(panier, nbArticles);

	// int panierCopy[nbArticles];
	// for (int i = 0; i < nbArticles; i++) { 
	// 	panierCopy[i] = panier[i];
	// }

	print_panier(G, panier, catalogue, nbArticles, print);


	algoBellman(G, start, tab);

	if(print)
		printf("\n\nprint bellman(%d)\n", start);
    // while (i<G->order) 
    // {
    //     printf("node%d: distance=%d, precedent:%d\n",i,tab[i].distance,tab[i].precedentNode);
    //     i++;
    // }
	printBellman(tab, G->order, print);

    pathToTarget(tab, start, 12, path, &nbPathNodes);
    printPathToTarget(path, nbPathNodes, start, 12, print);
	
	int *nodes = plus_court_chemin(G, panier, catalogue, start, tab, path, NbArticlesInPanier,nbArticles, print);

	print_path(G, path, panier, catalogue, nbArticles, print);

	return nodes;
}
	

int main()
{

	main_openGL();

    return 0;
}


// catalogue

void remplir_catalogue(Article* catalogue, char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(1);
    }

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
		token = strtok(NULL, " ");
        if (token != NULL) {
            int frozen_value = atoi(token);
            catalogue[i].frozen = (frozen_value == 1) ? 1 : 0;
        } else {
            // Gérer le cas où le champ frozen est manquant ou invalide dans le fichier
            catalogue[i].frozen = 0; // Valeur par défaut
        }


		i++;
	}
    fclose(fichier);

}
void print_catalogue(Article* catalogue, int nbArticles, int print) {
	if(print)
	{
		printf("\n\nCatalogue :\n\n");
		printf("+------+----------------------------+------------+--------+--------+------------+---------+\n");
		printf("| Ref. | Nom                        |   Prix     |  Poids | Volume | Resistance | surgele |\n");
		printf("+------+----------------------------+------------+--------+--------+------------+---------+\n");
		for (int i = 0; i < nbArticles; i++) {
			printf("| %4d | %-26s | %10.2f | %6.2f | %6.2f | %10.2f | %7d |\n",
				catalogue[i].ref, catalogue[i].nom, catalogue[i].prix, catalogue[i].poids, catalogue[i].volume, catalogue[i].resistance, catalogue->frozen);
		}
		printf("+------+----------------------------+------------+--------+--------+------------+---------+\n");
	}
}

















////////////////////////////////////////////////////
//    Representer le graphe avec OpenGL 		 ///
////////////////////////////////////////////////////















void drawSquare(int x, int y, int size, int bool_panier) {
    size = size *1.4; // Demi-taille du carré

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f); // Blanc
    glVertex2i(x - size, y - size);
    glVertex2i(x + size, y - size);
    glVertex2i(x + size, y + size);
    glVertex2i(x - size, y + size);
    glEnd();


	if (bool_panier) {
		glLineWidth(2.0); // Épaisseur du contour
		glColor3f(0.0f, 1.0f, 0.0f); // Vert
	} else {
		glLineWidth(2.0); // Épaisseur du contour
		glColor3f(0.0f, 0.0f, 0.0f); // Noir
	}
    glBegin(GL_LINE_LOOP);
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

void drawArrow(int x1, int y1, int x2, int y2) {
	// orange
	glColor3f(1.0f, 0.5f, 0.0f);
    // Calculer la longueur et l'angle du trait
    float dx = x1 - x2;
    float dy = y1 - y2;
    float length = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx);

    // Calculer les coordonnées des extrémités de la flèche
    float arrowLength = 15 * SIZE_OF_GRAPH; // Longueur de la flèche (10% de la longueur du trait)
    float arrowAngle = 0.5; // Angle d'ouverture de la flèche (0.5 radian)

    // Calculer les coordonnées du centre du trait
    float centerX = (x2 + x1) / 2.0;
    float centerY = (y2 + y1) / 2.0;

    // Calculer les coordonnées de la pointe de la flèche
    float arrowX = centerX - arrowLength * cos(angle); 
    float arrowY = centerY - arrowLength * sin(angle); 

    // Calculer les coordonnées des deux extrémités de la base de la flèche
    float arrowX1 = arrowX + arrowLength * cos(angle + arrowAngle);
    float arrowY1 = arrowY + arrowLength * sin(angle + arrowAngle);
    float arrowX2 = arrowX + arrowLength * cos(angle - arrowAngle);
    float arrowY2 = arrowY + arrowLength * sin(angle - arrowAngle);

    // Dessiner le trait
    glBegin(GL_LINES);
    glVertex2i(x2, y2);
    glVertex2i(x1, y1);
    glEnd();

    // Dessiner la flèche
    glBegin(GL_TRIANGLES);
    glVertex2f(arrowX, arrowY);
    glVertex2f(arrowX1, arrowY1);
    glVertex2f(arrowX2, arrowY2);
    glEnd();

	
}

void draw_itterator_path(int x, int y, int itterator_path) {
    glColor3f(1.0, 0.0, 0.0); // Rouge

    glBegin(GL_BITMAP);
    glRasterPos2i(x + 20, y - 20);

    char itterator_path_str[10];
    sprintf(itterator_path_str, "%d", itterator_path);
    int len = strlen(itterator_path_str);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, itterator_path_str[i]);
    }

    glEnd();
}



// Fonction d'affichage
void display() {  
	glClear(GL_COLOR_BUFFER_BIT);
	// Creer le Magasin :
	struct Graph *G = createGraph(0);
	int nbArticles = getNbArticles("bdd/catalogue.txt");
    int *path = malloc(512 * sizeof(int)); 
	int *panier = malloc(nbArticles * sizeof(int));
    Article *catalogue = malloc(nbArticles * sizeof(Article));

    struct eltBellman *tab = malloc(512 * sizeof(struct eltBellman));
	// remplir le panier, le catalogue, le tableau de Bellman et chemin (path[]) :
	int *nodes = creer_chemin(G, path, panier, catalogue, tab,nbArticles, PRINT);

	// len_panier : nombre d'articles dans le panier
	int len_panier = 0;
	for (int i = 0; i < nbArticles; i++) {
		if (panier[i] != -1) {
			len_panier++;
		}
	}

	// tableau de marquage des couples de noeuds déjà reliés par un trait orange
	int tab_marquage[512][512];
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			tab_marquage[i][j] = 0;
		}
	}
	
	float scale = SIZE_OF_GRAPH;
	// Dessiner les traits pour les liens
	glColor3f(1.0, 0.0, 0.0); // Rouge
	for (int i = 0; i < G->order; i++) {
		struct Node *node = G->tableNodes[i];
		for (int j = 0; j < node->nbAdj; j++) { 
			struct Node *adjNode = node->adjTab[j];

			// path : tableau de 512 cases contenant le chemin le plus court, toutes les cases sont initialisées à -1 (case vide) et seulement les cases remplies sont prises en compte
			// si node->nodeID et adjNode->nodeID se suivent dans le tableau path[] :
			// faire le trait en orange
			int bool_chemin = 0;
			int bool_arrow = 0;
			int bool_itterator_path = 0;
			int k = 0;
			float X1 = node->X * 50 * scale;
			float Y1 = node->Y * 40 * scale;
			float X2 = adjNode->X * 50 * scale;
			float Y2 = adjNode->Y * 40 * scale;




			while (!bool_chemin && k < 511 && path[k+1] != -1) {
				if ((path[k] == node->nodeID && path[k+1] == adjNode->nodeID)  && tab_marquage[node->nodeID][adjNode->nodeID] == 0 && node->boolShelf != 1 && adjNode->boolShelf != 1) {
					bool_arrow = 1;
					tab_marquage[node->nodeID][adjNode->nodeID] = 1;
					tab_marquage[adjNode->nodeID][node->nodeID] = 1;
				}
				if((path[k] == adjNode->nodeID && path[k+1] == node->nodeID) || (path[k] == node->nodeID && path[k+1] == adjNode->nodeID) ) {
					bool_chemin = 1;
				}
				if(path[k] == node->nodeID && node->boolShelf ==1 && (panier[node->productsTab[0]] != -1 || panier[node->productsTab[1]] !=-1 || panier[node->productsTab[2]] !=-1 )) {
					bool_itterator_path = 1;
					// printf("
				}
				k++;
			}
			
			if (bool_arrow) {
				glColor3f(1.0, 0.5, 0.0); 
                // Afficher la flèche
				drawArrow(X1, Y1, X2, Y2);
			}else if (bool_chemin) {
				// Modifier l'épaisseur de la ligne
				glLineWidth(5);
				// orange pour le chemin
				glColor3f(1.0, 0.5, 0.0); 
			}
			else{
				// Si lien entre rayons
				if ((adjNode->boolShelf == 0 && node->boolShelf == 0) || (adjNode->boolShelf == 2 && node->boolShelf == 0) || (adjNode->boolShelf == 0 && node->boolShelf == 2)) {
					// Modifier l'épaisseur de la ligne
						glLineWidth(2);
					// gris pour les liens entre rayons
					glColor3f(0.9, 0.9, 0.9);
				}else{
					// Modifier l'épaisseur de la ligne
						glLineWidth(5);
					// noir pour les liens entre article
					glColor3f(0.0, 0.0, 0.0); 
				}
			}
			drawLine(X1, Y1, X2, Y2);
			
		}
	}

	int vecteur_passage[512];
	for (int i = 0; i < 512; i++) {
		vecteur_passage[i] = 0;
	}
	for(int i = 0; i < len_panier; i++){
		if(vecteur_passage[nodes[i]] == 0){
			draw_itterator_path(G->tableNodes[nodes[i]]->X * 50 * scale, G->tableNodes[nodes[i]]->Y * 40 * scale, i+1);
		}
		else{
			draw_itterator_path((G->tableNodes[nodes[i]]->X +0.25)* 50 * scale, G->tableNodes[nodes[i]]->Y * 40 * scale, -(i+1));
		}
		vecteur_passage[nodes[i]]++;
	}



	// Dessiner les carrés pour les catalogues
	glColor3f(0.0, 0.0, 1.0); // Bleu
	int size = 7*scale;
	for (int i = 0; i < G->order; i++) {
		struct Node *node = G->tableNodes[i];
		int x = node->X * 50 * scale;
		int y = node->Y * 40 * scale;

		// le noeud est il dans le panier ?
		int r = 0;
		int bool_panier = 0;
		int somme = 0; 
		while (r<3) {
			if (node->boolShelf == 1 && panier[node->productsTab[r]] != -1 && r<node->nbProducts) {
				bool_panier = 1;
			}
			r++; 
		}

		
	    drawSquare(x, y, size, bool_panier); 

		// Afficher le numéro du nœud
		glColor3f(0.0, 0.0, 0.0); // Blanc
		glRasterPos2i(x - 6*scale, y - 6*scale);
		char nodeIDstr[10];
		sprintf(nodeIDstr, "%d", G->tableNodes[i]->nodeID);
		int len = strlen(nodeIDstr);
		for (int i = 0; i < len; i++) {
			// differents Helvetica en fonction de scale
			if(scale > 1.5){
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, nodeIDstr[i]);
			}else if(scale > 1.2){
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, nodeIDstr[i]);
			}else{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, nodeIDstr[i]);
			}
		}
		
	}
	// Rafraîchir l'affichage
	glutSwapBuffers();

	free(tab);
	free(path);
	free(nodes);
	free(panier);
	free(catalogue);

}

	// Fonction d'initialisation
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0); // Blanc
}

void reshape(int w, int h) {
    int x_origin = 100;
    int y_origin = 100;
    int newWidth = 1600; // Nouvelle largeur de la fenêtre
    int newHeight = 900; // Nouvelle hauteur de la fenêtre

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-x_origin, newWidth, -y_origin, newHeight);
    glMatrixMode(GL_MODELVIEW);
}


int main_openGL() {
    int argc = 1;
    char *argv[1] = { (char*)"glut" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1600, 900); // Nouvelles dimensions de la fenêtre
    glutCreateWindow("Magasin");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
