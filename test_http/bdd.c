#include <stdio.h>
#include <curl/curl.h>

// Callback function pour écrire les données téléchargées dans un fichier
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    FILE *file = (FILE *)userp;
    if (file) {
        size_t written = fwrite(buffer, size, nmemb, file);
        return written;
    }
    return 0;
}

int main() {
    CURL *curl;
    CURLcode res;
    FILE *file;
    const char *url = "https://perfectcourse.000webhostapp.com/catalogue.txt";
    const char *filename = "catalogue.txt";

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        file = fopen(filename, "wb");
        if (file) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                printf("Erreur lors du téléchargement : %s\n", curl_easy_strerror(res));
            } else {
                printf("Téléchargement terminé avec succès.\n");
            }

            fclose(file);
        } else {
            printf("Erreur lors de l'ouverture du fichier en écriture.\n");
        }

        curl_easy_cleanup(curl);
    } else {
        printf("Erreur lors de l'initialisation de libcurl.\n");
    }

    curl_global_cleanup();

    return 0;
}
