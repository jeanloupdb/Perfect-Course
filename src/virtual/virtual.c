#include <stdio.h>
#include "SDL/SDL.h"
#include "tool_img.h"

#include "SDL/SDL_image.h"


int main(int argc, char *argv[])
{
    // initialize SDL
    init_sdl();

    // load image
    SDL_Surface* img = load_image("mgs.jpg");

    // display image
    SDL_Surface* screen = display_image(img);

    // wait for key press
    wait_for_keypressed();

    // free memory
    SDL_FreeSurface(img);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
