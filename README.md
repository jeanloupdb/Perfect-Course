# Perfect-Course

... PROJET EN COURS ...

https://perfectcourse.neocities.org/

Votre allié pour optimiser votre temps et vos forces


Installation des bibliotheques : 
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install build-essential
sudo apt-get install libgl1-mesa-dev
sudo apt-get install libglu1-mesa-dev
sudo apt-get install freeglut3-dev



build/: contient le Makefile et tous les fichiers binaires générés après la compilation du code source.

include/: contient tous les fichiers d'en-tête (.h) utilisés dans le projet. Dans ce cas-ci, il contient un sous-dossier nommé graph qui contient les fichiers d'en-tête pour le module de graph.

src/: contient tous les fichiers sources (.c) du projet. Il est subdivisé en plusieurs sous-dossiers pour organiser le code selon les différents modules du projet. Ces sous-dossiers sont :

- article/: contient les fichiers source et en-tête pour le module d'articles.

- employee/: contient les fichiers source et en-tête pour le module d'employés.

- pathfinding/: contient les fichiers source et en-tête pour le module de pathfinding. Ce sous-dossier est lui-même subdivisé en deux sous-dossiers :

- dijkstra/: contient les fichiers source et en-tête pour l'algorithme de Dijkstra utilisé dans le module de pathfinding.

- graph/: contient les fichiers source et en-tête pour le module de graph utilisé dans le module de pathfinding.

- qr_code/: contient les fichiers source et en-tête pour le module de QR code.

- warehouse/: contient les fichiers source et en-tête pour le module d'entrepôt.

test/: contient tous les fichiers nécessaires pour les tests unitaires du projet.
