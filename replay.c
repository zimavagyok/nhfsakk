#include "Init/Initialize.h"
#include "Control/FileOperations.h"
#include "View/Display.h"
#include "Extensions/checkFunctions.h"
#include "Control/chess.h"
#include "Model/Moves.h"
#include "Model/ChessPieceList.h"
#include "Model/ChessBoard.h"
#include "main.h"
#include "debugmalloc.h"

void replay(char *filePath)
{
    ChessBoard *cb = initChessBoard();
    Replay replayMoves = {.movementCount = 0,.movement = NULL};
    int ret = loadMovesFromFile(&replayMoves,filePath);
    if(ret == 1 || replayMoves.movement == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","Wrong format!",window);
        FreePieces(cb->first);
        freeChessBoard(cb);
        return;
    }
    drawReplay(cb,renderer);

    int whichMove = 0;
    SDL_Event ev;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT)
    {
        switch(ev.type)
            {
            SDL_Point mousePos;
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(pointInRectangle(mousePos,350,436,400,486))
                {
                    if(whichMove<replayMoves.movementCount)
                    {
                        doMoveReplay(replayMoves.movement[whichMove++],cb);
                        drawReplay(cb,renderer);
                    }
                    else
                    {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"End","There is no more step!",window);
                    }
                }
                else if(pointInRectangle(mousePos,280,436,330,486))
                {
                    freeMovement(&replayMoves);
                    FreePieces(cb->first);
                    freeChessBoard(cb);
                    cb = initChessBoard();
                    replayMoves = (Replay){.movementCount = 0,.movement = NULL};
                    int ret = loadMovesFromFile(&replayMoves,filePath);
                    if(ret == 1 || replayMoves.movement == NULL)
                    {
                        return;
                    }
                    drawReplay(cb,renderer);

                    whichMove = 0;
                }
                else if(pointInRectangle(mousePos,20,436,100,486))
                {
                    freeMovement(&replayMoves);
                    FreePieces(cb->first);
                    freeChessBoard(cb);
                    return;
                }
                break;
            }
    }
}
