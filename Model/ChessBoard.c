#include "Entity.h"

tempBoard *CopyChessBoard(const ChessBoard *oldCb)
{
    tempBoard *newCb = malloc(sizeof(tempBoard));
    newCb->next = oldCb->next;
    for(int i =0;i<MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            if(oldCb->square[i][j].Piece!=NULL)
            {
                newCb->squares[i][j].color = oldCb->square[i][j].Piece->color;
                newCb->squares[i][j].type = oldCb->square[i][j].Piece->type;
            }
            else
            {
                newCb->squares[i][j].color = NONE;
                newCb->squares[i][j].type = NONE;
            }
            newCb->squares[i][j].File = oldCb->square[i][j].File;
            newCb->squares[i][j].Rank = oldCb->square[i][j].Rank;
            newCb->squares[i][j].squarePos = oldCb->square[i][j].squarePos;
        }
    }

    return newCb;
}
