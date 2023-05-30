#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

struct Matrix
{
    int width;
    int height;
    int** matrix;
};

struct Matrix* initMatrix(int width, int height)
{
    struct Matrix* matrix = malloc(sizeof(struct Matrix));
    matrix->width = width;
    matrix->height = height;
    matrix->matrix = malloc(sizeof(int*) * height);
    for (int i = 0; i < height; i++)
    {
        matrix->matrix[i] = malloc(sizeof(int) * width);
    }
    return matrix;
}

void freeMatrix(struct Matrix* matrix)
{
    for (int i = 0; i < matrix->height; i++)
    {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    free(matrix);
}

void printMatrix(struct Matrix* matrix)
{
    for (int i = 0; i < matrix->height; i++)
    {
        for (int j = 0; j < matrix->width; j++)
        {
            printf("%d ", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}

//On transforme la sdl image en matrice de 0 et 1 selon si le pixel est noir ou blanc
struct Matrix* imageToMatrix(SDL_Surface* image)
{
    struct Matrix* matrix = initMatrix(image->w, image->h);
    Uint8 r, g, b;
    Uint32 pixel;
    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            pixel = getpixel(image, j, i);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);
            if (r == 0 && g == 0 && b == 0)
            {
                matrix->matrix[i][j] = 1;
            }
            else
            {
                matrix->matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}