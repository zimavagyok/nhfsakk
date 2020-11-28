#include "../Init/Initialize.h"
#include "../main.h"
#include <SDL2/SDL_image.h>
#include "../debugmalloc.h"

ChessPiece *AppendPiece(ChessPiece *first,const PieceType type,const Color color)
{
    ChessPiece *newPiece = malloc(sizeof(ChessPiece));
    newPiece->color = color;
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

void UpgradePawn(ChessBoard *cb, const Coordinate where)
{
    bool found = false;
    ChessPiece *moving = cb->first;
    while(moving!=NULL && !found)
    {
        if(moving->color == cb->square[where.Rank][where.File].Piece->color && moving->type == QUEEN)
        {
            cb->square[where.Rank][where.File].Piece = moving;
        }
        moving = moving->next;
    }
    if(!found)
    {
        cb->first = AppendPiece(cb->first,QUEEN,cb->square[where.Rank][where.File].Piece->color);
        cb->square[where.Rank][where.File].Piece = cb->first;
    }
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
