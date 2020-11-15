#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Model/Entity.h"
#include "View/Display.h"
#include "Init/Initialize.h"
#include "Control/chess.h"
#include <math.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;

int main(int argc, char *argv[]) {
    sdl_initialize();
    ChessBoard *cb = initChessBoard();
    drawGame(cb,renderer);

    bool drag = false,game = true;
    Square *selected = NULL;
    Coordinate from;
    SDL_Point mousePos,clickOffset;
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT && game) {
            switch(ev.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                for(int i = 0;i<MAX_ROW;i++)
                {
                    for(int j = 0;j<MAX_COL;j++)
                    {
                        if(SDL_PointInRect(&mousePos,&cb->square[i][j].squarePos) && cb->square[i][j].Piece != NULL && cb->square[i][j].Piece->color == cb->next)
                        {
                            selected = &cb->square[i][j];
                            from.Rank = i;
                            from.File = j;
                            clickOffset.x = mousePos.x - cb->square[i][j].squarePos.x;
                            clickOffset.y = mousePos.y - cb->square[i][j].squarePos.y;
                            drag = true;
                            break;
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(drag == true && selected != NULL)
                {
                    if(mousePos.y>415)
                    {
                        mousePos.y = 415;
                    }
                    selected->Piece->image_position.x = mousePos.x - clickOffset.x;
                    selected->Piece->image_position.y = mousePos.y - clickOffset.y;
                    drawGame(cb,renderer);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(drag == true && selected != NULL)
                {
                    for(int i = 0;i<MAX_ROW;i++)
                    {
                        for(int j = 0;j<MAX_COL;j++)
                        {
                            if(SDL_PointInRect(&mousePos,&cb->square[i][j].squarePos))
                            {
                                Coordinate to = {.Rank = i,.File = j};
                                if((cb->square[i][j].Rank != selected->Rank || selected->File != to.File) && doMove(from,to,cb))
                                {
                                    cb->next = cb->next == WHITE ? BLACK : WHITE;
                                    SDL_RenderClear(renderer);
                                    if(checkCheckMate(cb))
                                    {
                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"WIN",cb->next==WHITE? "Black wins!" : "White wins!",window);
                                        game = false;
                                    }
                                }
                                else
                                {
                                    selected->Piece->image_position = selected->squarePos;
                                    drawGame(cb,renderer);
                                }
                                drawGame(cb,renderer);
                                break;
                            }
                        }
                    }
                }
                drag = false;
                selected = NULL;
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym) {
                    case SDLK_ESCAPE: game = false;
                }
                break;
            }
    }

    FreePieces(cb->first);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
