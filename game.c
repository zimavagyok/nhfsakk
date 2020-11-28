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

    bool game = true, moveHappened = false;
    Square *selected = NULL;
    Coordinate from;
    SDL_Point mousePos;
    SDL_Event ev;
    Move *movesInGame = NULL;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT && game) {
            switch(ev.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(pointInRectangle(mousePos,20,436,100,486))
                {
                    game = false;
                    if(moveHappened)
                        WriteToFile(movesInGame,cb,false);
                }
                if(selected != NULL)
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
                                    moveHappened = true;
                                    movesInGame = addMoveToHistory(movesInGame,from,to);
                                    cb->next = cb->next == WHITE ? BLACK : WHITE;
                                    SDL_RenderClear(renderer);
                                    if(checkCheckMate(cb))
                                    {
                                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"WIN",cb->next==WHITE? "Black wins!" : "White wins!",window);
                                        game = false;
                                        if(!cb->loaded)
                                            WriteToFile(movesInGame,cb,true);
                                        break;
                                    }
                                }
                                else
                                {
                                    drawGame(cb,renderer);
                                }
                                drawGame(cb,renderer);
                                break;
                            }
                        }
                    }
                    selected = NULL;
                }
                else if(selected==NULL)
                {
                    for(int i = 0;i<MAX_ROW;i++)
                    {
                        for(int j = 0;j<MAX_COL;j++)
                        {
                            if(SDL_PointInRect(&mousePos,&cb->square[i][j].squarePos) && cb->square[i][j].Piece != NULL && cb->square[i][j].Piece->color == cb->next)
                            {
                                selected = &cb->square[i][j];
                                from.Rank = i;
                                from.File = j;
                                Moves availableMoves;
                                LegalMoves(from,cb,&availableMoves);
                                if(availableMoves.coordinates != NULL)
                                {
                                    showAvailableMoves(renderer,availableMoves);
                                    freeMoves(&availableMoves);
                                }
                                else
                                    selected = NULL;

                                break;
                            }
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game = false;
                        if(moveHappened)
                            WriteToFile(movesInGame,cb,false);
                }
                break;
            }
    }

    FreePieces(cb->first);
    freeChessBoard(cb);
}
