#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


/*struct PlanStock
{
	int nbRangees;
	int nbCasiers;
	int produit[][];
};*/

struct Graph
{
	int order; /* nb de noeuds */
	struct Node *tableNodes[512];
};

struct Graph G;

struct Node 
{
	int boolShelf;/*si 0 -> intermediate, si 1 -> shelf, si 2 -> debut*/
	int aisle;
	int shelf;
	int boolTopBottom; /*si boolShelf = 0, btb = 1 si top, sinon 0*/
	struct Node *adjTab[10];
	int nbAdj;
	int X;
	int Y;
	int productsTab[3];
	int nbProducts;
};

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
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_1,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,1));
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_1,"top") == 0))
			createLink(getNodeIntermediate(aisle_1,0),getNodeIntermediate(aisle_2,1));
		if ((strcmp(botop_1,"top") == 0) && (strcmp(botop_1,"bottom") == 0))
			createLink(getNodeIntermediate(aisle_1,1),getNodeIntermediate(aisle_2,0));
		if ((strcmp(botop_1,"bottom") == 0) && (strcmp(botop_1,"bottom") == 0))
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


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////       IMPLEMENTATION GRAPHIQUE         //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

// Structure pour stocker les données d'un cercle
typedef struct {
    int x;
    int y;
    int r;
} Circle;

// Structure pour stocker les données d'un rayon
typedef struct {
    Circle origin; // Point de départ du rayon
    Circle target; // Point d'arrivée du rayon
} Ray;

// Fonction pour dessiner un cercle
void drawCircle(SDL_Renderer *renderer, Circle *circle) {
    for(int i = 0; i < circle->r * 2; i++) {
        for(int j = 0; j < circle->r * 2; j++) {
            int dx = circle->r - i;
            int dy = circle->r - j;
            if((dx*dx + dy*dy) <= (circle->r * circle->r)) {
                SDL_RenderDrawPoint(renderer, circle->x + dx, circle->y + dy);
            }
        }
    }
}

// Fonction pour dessiner un rayon
void drawRay(SDL_Renderer *renderer, Ray *ray) {
    SDL_RenderDrawLine(renderer, ray->origin.x, ray->origin.y, ray->target.x, ray->target.y);
}

// Fonction pour dessiner le graph
void drawGraph(SDL_Renderer *renderer, struct Graph *graph) {
    // Dessiner chaque noeud sous forme de cercle
    for(int i = 0; i < graph->order; i++) {
        Circle circle = {graph->tableNodes[i]->X, graph->tableNodes[i]->Y, 10};
        drawCircle(renderer, &circle);
    }
    // Dessiner chaque lien entre les noeuds sous forme de rayon
    for(int i = 0; i < graph->order; i++) {
        struct Node *node = graph->tableNodes[i];
        for(int j = 0; j < node->nbAdj; j++) {
            struct Node *adj = node->adjTab[j];
            Ray ray = {{node->X, node->Y, 10}, {adj->X, adj->Y, 10}};
            drawRay(renderer, &ray);
        }
    }
}

// Fonction principale
int main(int argc, char *argv[]) {
    // Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);
    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Magasin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    // Création du renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Fond blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    // Dessiner le graph
    drawGraph(renderer, &G);
    // Afficher la fenêtre
    SDL_RenderPresent(renderer);
    // Attendre que l'utilisateur quitte
    SDL_Event event;
    while(SDL_WaitEvent(&event) >= 0) {
        if(event.type == SDL_QUIT) {
            break;
        }
    }
    // Libérer la mémoire
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
