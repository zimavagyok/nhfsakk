#include "View/Display.h"
#include "main.h"
#include "loadMenu.h"
#include "Model/Entity.h"
#include "Extensions/checkFunctions.h"
#include "game.h"
#include "debugmalloc.h"

void menu()
{
    drawMenu(renderer);
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        SDL_Point mousePos;
        switch(ev.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(pointInRectangle(mousePos,130,130,286,180))
                {
                    game(false,NULL);
                    drawMenu(renderer);
                }
                else if(pointInRectangle(mousePos,130,200,286,250))
                {
                    loadMenu();
                    drawMenu(renderer);
                }
                else if(pointInRectangle(mousePos,130,270,286,320))
                {
                    return;
                }
            }

    }
}
