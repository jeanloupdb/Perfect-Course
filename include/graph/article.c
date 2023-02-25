#include <stdlib.h>
#include <string.h>
#include "article.h"

// Création d'un article
Article* create_article(char* nom, float prix, float poids, float volume, float resistance) {
    Article* article = malloc(sizeof(Article));
    if (article == NULL) {
        return NULL;
    }
    article->nom = malloc(strlen(nom) + 1);
    if (article->nom == NULL) {
        free(article);
        return NULL;
    }
    strcpy(article->nom, nom);
    article->prix = prix;
    article->poids = poids;
    article->volume = volume;
    article->resistance = resistance;
    return article;
}

// Destruction d'un article
void destroy_article(Article* article) {
    free(article->nom);
    free(article);
}
