#include "../Init/Initialize.h"
#include "../main.h"
#include <SDL2/SDL_image.h>
#include "../debugmalloc.h"

void UpgradePawn(ChessBoard *cb, const Coordinate where)
{
    SDL_DestroyTexture(cb->square[where.Rank][where.File].Piece->image);
    cb->square[where.Rank][where.File].Piece->image = IMG_LoadTexture(renderer,cb->square[where.Rank][where.File].Piece->color==WHITE ? "assets/images/WQueen.png" : "assets/images/BQueen.png");
    cb->square[where.Rank][where.File].Piece->type = QUEEN;
}

/*ChessPiece *AppendPiece(ChessPiece *first,const int Rank,const int File,const PieceType type,const Color color,const char *fn)
{
    ChessPiece *newPiece = malloc(sizeof(ChessPiece));
    newPiece->alive = true;
    newPiece->color = color;
    newPiece->Rank = Rank;
    newPiece->File = File;
    newPiece->image_position = initPosByFileRank(Rank,File);
    newPiece->image = IMG_LoadTexture(renderer,fn);
    newPiece->type = type;
    if(first == NULL)
    {
        newPiece->next = NULL;
    }
    else
        newPiece->next = first;
    return newPiece;
}*/

ChessPiece *AppendPiece(ChessPiece *first,const int Rank,const int File,const PieceType type,const Color color)
{
    ChessPiece *newPiece = malloc(sizeof(ChessPiece));
    newPiece->alive = true;
    newPiece->color = color;
    newPiece->Rank = Rank;
    newPiece->File = File;
    newPiece->image_position = initPosByFileRank(Rank,File);
    switch(type)
    {
    case PAWN:
        if(color == WHITE)
        {
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WPawn.png");
        }
        else
        {
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BPawn.png");
        }
        break;
    case ROOK:
        if(color == WHITE)
        {
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WRook.png");
        }
        else
        {
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BRook.png");
        }
        break;
    case KNIGHT:
        if(color == WHITE)
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WKnight.png");
        else
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BKnight.png");
        break;
    case BISHOP:
        if(color == WHITE)
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WBishop.png");
        else
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BBishop.png");
        break;
    case QUEEN:
        if(color == WHITE)
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WQueen.png");
        else
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BQueen.png");
        break;
    case KING:
        if(color == WHITE)
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/WKing.png");
        else
            newPiece->image = IMG_LoadTexture(renderer,"assets/images/BKing.png");
        break;
    }
    newPiece->type = type;
    if(first == NULL)
    {
        newPiece->next = NULL;
    }
    else
        newPiece->next = first;
    return newPiece;
}

void FreePieces(ChessPiece *First)
{
    ChessPiece *current = First, *next;
    while(current)
    {
        SDL_DestroyTexture(current->image);
        next = current->next;
        free(current);
        current = next;
    }
}

ChessPiece *RemovePiece(ChessPiece *First, ChessPiece *which)
{
    ChessPiece *behind = NULL;
    ChessPiece *moving = First;
    while(moving != NULL && !SDL_RectEquals(&moving->image_position,&which->image_position))
    {
        behind = moving;
        moving = moving->next;
    }

    if(moving==NULL)
    {
        return First;
    }
    else if(behind == NULL)
    {
        ChessPiece *newFirst = moving->next;
        SDL_DestroyTexture(moving->image);
        free(moving);
        return newFirst;
    }
    else
    {
        behind->next = moving->next;
        SDL_DestroyTexture(moving->image);
        free(moving);
        return First;
    }
}
