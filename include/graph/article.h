#ifndef ARTICLE_H
#define ARTICLE_H

// Définition de la structure d'un article
typedef struct Article {
    char* nom;         // Nom de l'article
    float prix;        // Prix de l'article
    float poids;       // Poids de l'article
    float volume;      // Volume de l'article
    float resistance;  // Résistance de l'article
} Article;

#endif /* ARTICLE_H */
