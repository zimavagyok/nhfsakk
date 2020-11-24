#include "Model/Entity.h"
#include "View/Display.h"
#include "Init/Initialize.h"
#include "Control/chess.h"
#include "Control/FileOperations.h";
#include "Model/Entity.h"
#include "Model/ChessPieceList.h"
#include "Model/Moves.h"
#include "main.h"
#include "Extensions/checkFunctions.h"
#include <stdbool.h>
#include "debugmalloc.h"

void game(bool loadFromFile, char *file)
{
    ChessBoard *cb;
    if(!loadFromFile)
        cb = initChessBoard();
    else
        cb = initChessBoardFromFile(file);
    if(cb==NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Wrong format!",window);
        return;
    }
    drawGame(cb,renderer);

    bool drag = false,game = true;
    Square *selected = NULL;
    Coordinate from;
    SDL_Point mousePos,clickOffset;
    SDL_Event ev;
    Move *first = NULL;
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
                //20,436,100,486
                if(pointInRectangle(mousePos,20,436,100,486))
                {
                    game = false;
                    WriteToFile(first,cb,false);
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
                                    first = addMoveToHistory(first,from,to);
                                    cb->next = cb->next == WHITE ? BLACK : WHITE;
                                    SDL_RenderClear(renderer);
                                    if(checkCheckMate(cb))
                                    {
                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"WIN",cb->next==WHITE? "Black wins!" : "White wins!",window);
                                        game = false;
                                        if(!cb->loaded)
                                            WriteToFile(first,cb,true);
                                        break;
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
                    case SDLK_ESCAPE:
                        game = false;
                        WriteToFile(first,cb,false);
                }
                break;
            }
    }

    FreePieces(cb->first);
    free(cb);
}
