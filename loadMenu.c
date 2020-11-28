#include "View/Display.h"
#include "Extensions/checkFunctions.h"
#include "replay.h"
#include "game.h"
#include "main.h"
#include "debugmalloc.h"

void loadMenu()
{
    drawLoadMenu(renderer,false);

    char* fileDir = NULL;
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        switch(ev.type)
            {
            SDL_Point mousePos;
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(pointInRectangle(mousePos,130,130,286,180))
                {
                    if(fileDir == NULL)
                    {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","You have to select a file first!",window);
                    }
                    else
                    {
                        game(true,fileDir);
                        drawLoadMenu(renderer,fileDir == NULL ? false : true);
                    }
                }
                else if(pointInRectangle(mousePos,130,200,286,250))
                {
                    if(fileDir == NULL)
                    {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","You have to select a file first!",window);
                    }
                    else
                    {
                        replay(fileDir);
                        drawLoadMenu(renderer,fileDir == NULL ? false : true);
                    }
                }
                else if(pointInRectangle(mousePos,130,270,286,320))
                {
                    return;
                }
                break;
            case (SDL_DROPFILE):
                fileDir = ev.drop.file;
                drawLoadMenu(renderer,fileDir == NULL ? false : true);
                break;
            }
    }
}
