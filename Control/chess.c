#include "../Model/Entity.h"
#include "../Model/ChessBoard.h"
#include "../Init/Initialize.h"
#include "../Model/Moves.h"
#include "../Extensions/checkFunctions.h"
#include "../Model/ChessPieceList.h"
#include "../debugmalloc.h"

void doMoveReplay(const Movement replayMove, ChessBoard *cb)
{
    if(cb->square[replayMove.to.Rank][replayMove.to.File].Piece == NULL)
    {
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece = cb->square[replayMove.from.Rank][replayMove.from.File].Piece;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->image_position = cb->square[replayMove.to.Rank][replayMove.to.File].squarePos;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->Rank = replayMove.to.Rank;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->File = replayMove.to.File;
        cb->square[replayMove.from.Rank][replayMove.from.File].Piece = NULL;
    }
    else
    {
        cb->first = RemovePiece(cb->first,cb->square[replayMove.to.Rank][replayMove.to.File].Piece);
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece = cb->square[replayMove.from.Rank][replayMove.from.File].Piece;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->image_position = cb->square[replayMove.to.Rank][replayMove.to.File].squarePos;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->Rank = replayMove.to.Rank;
        cb->square[replayMove.to.Rank][replayMove.to.File].Piece->File = replayMove.to.File;
        cb->square[replayMove.from.Rank][replayMove.from.File].Piece = NULL;
    }
}

tempBoard *doMoveTemp(const Coordinate from, const Coordinate to, tempBoard *tb)
{
    tb->squares[to.Rank][to.File] = tb->squares[from.Rank][from.File];
    tb->squares[from.Rank][from.File].color = NONE;
    tb->squares[from.Rank][from.File].type = NONE;
    return tb;
}

bool checkCheck(const Coordinate from, const Coordinate to, const ChessBoard *cb)
{
    tempBoard *tb = CopyChessBoard(cb);
    if(tb->squares[to.Rank][to.File].type == KING)
    {
        free(tb);
        return false;
    }
    tb = doMoveTemp(from,to,tb);

    //get king's coordinates with the current color
    Coordinate kingCoo;
    for(int i = 0;i<MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            if(tb->squares[i][j].type == KING && tb->squares[i][j].color == cb->next)
            {
                kingCoo.Rank = i;
                kingCoo.File = j;
                break;
            }
        }
    }

    //check if king is in check starting from the king's location
    int knightRank[] = {2,1,-1,-2,-2,-1,1,2};
    int knightFile[] = {1,2,2,1,-1,-2,-2,-1};

    for(int i = 0;i<8;i++)
    {
        Coordinate movePos = {.Rank = kingCoo.Rank + knightRank[i],.File = kingCoo.File + knightFile[i]};
        if(insideBoarders(movePos) && !tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].color != cb->next && tb->squares[movePos.Rank][movePos.File].type == KNIGHT)
        {
            free(tb);
            return true;
        }
    }

    int rookMoveIdx[] = {1,-1};
    int Rank = kingCoo.Rank;
    int File = kingCoo.File;

    //HORIZONTAL AND VERTICAL
    for(int i = 0;i<2;i++)
    {
        for(int j = Rank + rookMoveIdx[i];j<MAX_ROW && j >= 0;j+=rookMoveIdx[i])
        {
            Coordinate movePos = {.Rank = j,.File = kingCoo.File};
            if(!tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].color != cb->next && (tb->squares[movePos.Rank][movePos.File].type == QUEEN || tb->squares[movePos.Rank][movePos.File].type == ROOK))
            {
                free(tb);
                return true;
            }
            else if(!tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].color == cb->next)
            {
                break;
            }
        }
        for(int j = File + rookMoveIdx[i];j<MAX_COL && j>= 0;j+=rookMoveIdx[i])
        {
            Coordinate movePos = {.Rank = kingCoo.Rank,.File = j};
            if(!tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].color != cb->next && (tb->squares[movePos.Rank][movePos.File].type == QUEEN || tb->squares[movePos.Rank][movePos.File].type == ROOK))
            {
                free(tb);
                return true;
            }
            else if(!tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].color == cb->next)
            {
                break;
            }
        }
    }

    //DIAGONAL
    for(int i = Rank + 1, j = File + 1; i<MAX_ROW && j<MAX_COL; i++, j++)
    {
        Coordinate movePos = {.Rank = i, .File = j};
        if(!tempSquareEmpty(movePos,tb))
        {
            if(tb->squares[i][j].color == cb->next)
            {
                break;
            }
            else if(tb->squares[i][j].type == BISHOP || tb->squares[i][j].type == QUEEN)
            {
                free(tb);
                return true;
            }
        }
    }
    for(int i = Rank - 1, j = File + 1; i>=0 && j<MAX_COL; i--, j++)
    {
        Coordinate movePos = {.Rank = i, .File = j};
        if(!tempSquareEmpty(movePos,tb))
        {
            if(tb->squares[i][j].color == cb->next)
            {
                break;
            }
            else if(tb->squares[i][j].type == BISHOP || tb->squares[i][j].type == QUEEN)
            {
                free(tb);
                return true;
            }
        }
    }
    for(int i = Rank + 1, j = File - 1; i<MAX_ROW && j>=0; i++, j--)
    {
        Coordinate movePos = {.Rank = i, .File = j};
        if(!tempSquareEmpty(movePos,tb))
        {
            if(tb->squares[i][j].color == cb->next)
            {
                break;
            }
            else if(tb->squares[i][j].type == BISHOP || tb->squares[i][j].type == QUEEN)
            {
                free(tb);
                return true;
            }
        }
    }
    for(int i = Rank - 1, j = File - 1; i<MAX_ROW && j>=0; i--, j--)
    {
        Coordinate movePos = {.Rank = i, .File = j};
        if(!tempSquareEmpty(movePos,tb))
        {
            if(tb->squares[i][j].color == cb->next)
            {
                break;
            }
            else if(tb->squares[i][j].type == BISHOP || tb->squares[i][j].type == QUEEN)
            {
                free(tb);
                return true;
            }
        }
    }

    //PAWN
    int pawnRank[] = {1,1,-1,-1};
    int pawnFile[] = {1,-1,1,-1};
    for(int i = 0;i<4;i++)
    {
        Coordinate movePos = {.Rank = Rank + pawnRank[i],.File = File + pawnFile[i]};
        if(insideBoarders(movePos) && !tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].type == PAWN && tb->squares[movePos.Rank][movePos.File].color != cb->next && tb->squares[movePos.Rank][movePos.File].Rank < Rank && cb->next == WHITE)
        {
            free(tb);
            return true;
        }
        else if(insideBoarders(movePos) && !tempSquareEmpty(movePos,tb) && tb->squares[movePos.Rank][movePos.File].type == PAWN && tb->squares[movePos.Rank][movePos.File].color != cb->next && tb->squares[movePos.Rank][movePos.File].Rank > Rank && cb->next == BLACK)
        {
            free(tb);
            return true;
        }
    }

    free(tb);
    return false;
}

bool addIfNotCheck(const Coordinate from, const Coordinate to, const ChessBoard *cb, Moves *legal)
{
    if(squareEmpty(to,cb) && !checkCheck(from,to,cb))
    {
        addMove(legal,to);
    }
    else if(!squareEmpty(to,cb) && cb->square[to.Rank][to.File].Piece->color == cb->next)
    {
        return true;
    }
    else if(!squareEmpty(to,cb) && cb->square[to.Rank][to.File].Piece->color != cb->next)
    {
        if(!checkCheck(from,to,cb))
            addMove(legal,to);
        return true;
    }
    return false;
}

void LegalMoves(const Coordinate from,const ChessBoard *cb, Moves *move)
{
    Moves legal = {.moveCount = 0,.coordinates = NULL};
    Square current = cb->square[from.Rank][from.File];
    switch(current.Piece->type)
    {
    case PAWN:
        if(current.Piece->color == WHITE)
        {
            if(current.Rank+1 < MAX_ROW && cb->square[current.Rank+1][current.File].Piece == NULL)
            {
                Coordinate to = {.Rank = current.Rank+1,.File = current.File};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank+2 < MAX_ROW && cb->square[current.Rank+2][current.File].Piece == NULL && current.Rank == 1 && cb->square[current.Rank+1][current.File].Piece == NULL)
            {
                Coordinate to = {.Rank = current.Rank+2,.File = current.File};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank+1 < MAX_ROW && current.File+1 < MAX_COL && cb->square[current.Rank+1][current.File+1].Piece != NULL && cb->square[current.Rank+1][current.File+1].Piece->color != cb->next)
            {
                Coordinate to = {.Rank = current.Rank+1,.File = current.File+1};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank+1 < MAX_ROW && current.File-1 >= 0 && cb->square[current.Rank+1][current.File-1].Piece != NULL && cb->square[current.Rank+1][current.File-1].Piece->color != cb->next)
            {
                Coordinate to = {.Rank = current.Rank+1,.File = current.File-1};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
        }
        else
        {
            if(current.Rank-1 >= 0 && cb->square[current.Rank-1][current.File].Piece == NULL)
            {
                Coordinate to = {.Rank = current.Rank-1,.File = current.File};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank-2 >= 0 && cb->square[current.Rank-2][current.File].Piece == NULL && current.Rank == 6 && cb->square[current.Rank-1][current.File].Piece == NULL)
            {
                Coordinate to = {.Rank = current.Rank-2,.File = current.File};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank-1 >= 0 && current.File+1 < MAX_COL && cb->square[current.Rank-1][current.File+1].Piece != NULL && cb->square[current.Rank-1][current.File+1].Piece->color != cb->next)
            {
                Coordinate to = {.Rank = current.Rank-1,.File = current.File+1};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
            if(current.Rank-1 < MAX_ROW && current.File-1 >= 0 && cb->square[current.Rank-1][current.File-1].Piece != NULL && cb->square[current.Rank-1][current.File-1].Piece->color != cb->next)
            {
                Coordinate to = {.Rank = current.Rank-1,.File = current.File-1};
                if(!checkCheck(from,to,cb))
                {
                    addMove(&legal,to);
                }
            }
        }
        break;
    case KNIGHT: ;
        int knightRank[] = {2,1,-1,-2,-2,-1,1,2};
        int knightFile[] = {1,2,2,1,-1,-2,-2,-1};

        for(int i = 0;i<8;i++)
        {
            Coordinate to = {.Rank = current.Rank+knightRank[i],.File = current.File+knightFile[i]};
            if(insideBoarders(to) && !checkCheck(from,to,cb) && (cb->square[to.Rank][to.File].Piece == NULL || cb->square[to.Rank][to.File].Piece->color != cb->next))
            {
                addMove(&legal,to);
            }
        }
        break;
    case ROOK:;
        bool stop;
        int rookIndex[] = {1,-1};

        for(int i = 0;i<2;i++)
        {
            stop = false;
            for(int j = current.Rank + rookIndex[i];j<MAX_ROW && j>=0 && !stop ;j+=rookIndex[i])
            {
                Coordinate to = {.Rank = j,.File=current.File};
                stop = addIfNotCheck(from,to,cb,&legal);
            }
            stop = false;
            for(int j = current.File +rookIndex[i];j<MAX_COL && j>=0 && !stop;j+=rookIndex[i])
            {
                Coordinate to = {.Rank = current.Rank,.File=j};
                stop = addIfNotCheck(from,to,cb,&legal);
            }
        }
        break;
    case BISHOP:;
        stop = false;
        for(int i = current.Rank + 1, j = current.File +1; i<MAX_ROW && j<MAX_COL && !stop;i++,j++)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank + 1, j = current.File -1; i<MAX_ROW && j>=0 && !stop;i++,j--)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank - 1, j = current.File +1; i>=0 && j<MAX_COL && !stop;i--,j++)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank - 1, j = current.File - 1; i>=0 && j>=0 && !stop;i--,j--)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        break;
    case QUEEN:;
        int index[] = {1,-1};

        //VERTIVAL & HORIZONTAL
        for(int i = 0;i<2;i++)
        {
            stop = false;
            //int queenRank = current.Rank + index[i];
            for(int j = current.Rank + index[i];j<MAX_ROW && j>=0 && !stop ;j+=index[i])
            {
                Coordinate to = {.Rank = j,.File=current.File};
                stop = addIfNotCheck(from,to,cb,&legal);
            }
            stop = false;
            for(int j = current.File +index[i];j<MAX_COL && j>=0 && !stop;j+=index[i])
            {
                Coordinate to = {.Rank = current.Rank,.File=j};
                stop = addIfNotCheck(from,to,cb,&legal);
            }
        }

        //DIAGONAL
        stop = false;
        for(int i = current.Rank + 1, j = current.File +1; i<MAX_ROW && j<MAX_COL && !stop;i++,j++)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank + 1, j = current.File -1; i<MAX_ROW && j>=0 && !stop;i++,j--)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank - 1, j = current.File +1; i>=0 && j<MAX_COL && !stop;i--,j++)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        stop = false;
        for(int i = current.Rank - 1, j = current.File - 1; i>=0 && j>=0 && !stop;i--,j--)
        {
            Coordinate to = {.Rank = i, .File=j};
            stop = addIfNotCheck(from,to,cb,&legal);
        }
        break;
    case KING:;
        int kingRank[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
        int kingFile[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        for(int i = 0;i<8;i++)
        {
            Coordinate to = {.Rank = current.Rank + kingRank[i],.File = current.File + kingFile[i]};
            if(insideBoarders(to) && (cb->square[to.Rank][to.File].Piece == NULL || cb->square[to.Rank][to.File].Piece->color != cb->next) && !checkCheck(from,to,cb))
            {
                addMove(&legal,to);
            }
        }
        break;
    }

    move->coordinates = legal.coordinates;
    move->moveCount = legal.moveCount;
}

bool doMove(const Coordinate from,const Coordinate to, ChessBoard *cb)
{
    Moves allMoves;
    allMoves.moveCount = 0;
    allMoves.coordinates = NULL;
    LegalMoves(from,cb,&allMoves);

    bool available = false;
    for(int i =0;i<allMoves.moveCount;i++)
    {
        if(compareCoordinates(to,allMoves.coordinates[i]))
        {
            available = true;
            break;
        }
    }

    if(!available)
    {
        cb->square[from.Rank][from.File].Piece->image_position = cb->square[from.Rank][from.File].squarePos;
        freeMoves(&allMoves);
        return false;
    }

    if(cb->square[to.Rank][to.File].Piece == NULL)
    {
        cb->square[to.Rank][to.File].Piece = cb->square[from.Rank][from.File].Piece;
        cb->square[to.Rank][to.File].Piece->image_position = cb->square[to.Rank][to.File].squarePos;
        cb->square[to.Rank][to.File].Piece->Rank = to.Rank;
        cb->square[to.Rank][to.File].Piece->File = to.File;
        cb->square[from.Rank][from.File].Piece = NULL;
    }
    else
    {
        cb->first = RemovePiece(cb->first,cb->square[to.Rank][to.File].Piece);
        cb->square[to.Rank][to.File].Piece = cb->square[from.Rank][from.File].Piece;
        cb->square[to.Rank][to.File].Piece->image_position = cb->square[to.Rank][to.File].squarePos;
        cb->square[to.Rank][to.File].Piece->Rank = to.Rank;
        cb->square[to.Rank][to.File].Piece->File = to.File;
        cb->square[from.Rank][from.File].Piece = NULL;
    }
    if((to.Rank == 0 || to.Rank == 7) && cb->square[to.Rank][to.File].Piece->type == PAWN)
    {
        UpgradePawn(cb,to);
    }
    freeMoves(&allMoves);
    return true;
}

bool checkCheckMate(const ChessBoard *cb)
{
    ChessPiece *moving;

    for(moving = cb->first;moving != NULL;moving = moving->next)
    {
        if(moving->color == cb->next)
        {
            Coordinate from = {.Rank = moving->Rank,.File = moving->File};
            Moves availableMoves;
            availableMoves.moveCount = 0;
            availableMoves.coordinates = NULL;
            LegalMoves(from,cb,&availableMoves);
            if(availableMoves.moveCount != 0)
            {
                freeMoves(&availableMoves);
                return false;
            }
            freeMoves(&availableMoves);
        }
    }
    return true;
}
