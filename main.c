#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Model/Entity.h"
#include "View/Display.h"
#include "Init/Initialize.h"
#include "Control/chess.h"
#include "game.h"
#include <math.h>
#include "menu.h"
#include <stdlib.h>
#include <unistd.h>
#include "debugmalloc.h"

SDL_Window *window;
SDL_Renderer *renderer;

int main(int argc, char *argv[]) {
    sdl_initialize();
    menu();

    //game(false,NULL);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
