#include "../Init/Initialize.h"
#include "../main.h"
#include <SDL2/SDL_image.h>

ChessPiece *AppendPiece(ChessPiece *first,const int Rank,const int File,const PieceType type,const Color color,const char *fn)
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
