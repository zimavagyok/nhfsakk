#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include "../main.h"
#include "../Model/Entity.h"
#include "../Model/ChessPieceList.h"
#include <stdio.h>
#include "../debugmalloc.h"

void sdl_initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 416, 500, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);
}

SDL_Rect initPosByFileRank(int Rank, int File)
{
    SDL_Rect rectangle = {.x=File*SIZE,.y=Rank*SIZE,.h = SIZE, .w = SIZE};
    return rectangle;
}

ChessBoard *initChessBoard()
{
    ChessBoard *board = malloc(sizeof(ChessBoard));

    for(int i = 0;i< MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            board->square[i][j].Rank = i;
            board->square[i][j].File = j;
            board->square[i][j].Piece = NULL;
            board->square[i][j].squarePos = initPosByFileRank(i,j);
        }
    }
    board->loaded = false;
    board->board = IMG_LoadTexture(renderer,"assets/images/chessBoard.png");
    ChessPiece *first = NULL;

    //WHITE
    first = AppendPiece(first,PAWN,WHITE);
    for(int j = 0;j<8;j++)
    {
        board->square[1][j].Piece = first;
    }

    first=AppendPiece(first,ROOK,WHITE);
    board->square[0][0].Piece = first;
    board->square[0][7].Piece = first;
    first=AppendPiece(first,KNIGHT,WHITE);
    board->square[0][1].Piece = first;
    board->square[0][6].Piece = first;
    first=AppendPiece(first,BISHOP,WHITE);
    board->square[0][2].Piece = first;
    board->square[0][5].Piece = first;
    first=AppendPiece(first,KING,WHITE);
    board->square[0][3].Piece = first;
    first=AppendPiece(first,QUEEN,WHITE);
    board->square[0][4].Piece = first;

    //BLACK
    first = AppendPiece(first,PAWN,BLACK);
    for(int j = 0;j<8;j++)
    {
        board->square[6][j].Piece = first;
    }

    first=AppendPiece(first,ROOK,BLACK);
    board->square[7][0].Piece = first;
    board->square[7][7].Piece = first;
    first=AppendPiece(first,KNIGHT,BLACK);
    board->square[7][1].Piece = first;
    board->square[7][6].Piece = first;
    first=AppendPiece(first,BISHOP,BLACK);
    board->square[7][2].Piece = first;
    board->square[7][5].Piece = first;
    first=AppendPiece(first,KING,BLACK);
    board->square[7][3].Piece = first;
    first=AppendPiece(first,QUEEN,BLACK);
    board->square[7][4].Piece = first;

    board->first = first;
    board->next = WHITE;

    return board;
}

ChessBoard *initChessBoardFromFile(char *file)
{
    ChessBoard *board = malloc(sizeof(ChessBoard));

    for(int i = 0;i< MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            board->square[i][j].Rank = i;
            board->square[i][j].File = j;
            board->square[i][j].Piece = NULL;
            board->square[i][j].squarePos = initPosByFileRank(i,j);
        }
    }
    board->loaded = true;
    board->board = IMG_LoadTexture(renderer,"assets/images/chessBoard.png");
    ChessPiece *first = NULL;

    FILE *fp = fopen(file,"r");
    if(fp == NULL)
    {
        free(board);
        return NULL;
    }
    char header[11];
    fscanf(fp,"%s[\n]",header);
    if(strcmp(header,"Chess:Save")!=0)
    {
        free(board);
        return NULL;
    }

    int nextColor;
    fscanf(fp,"%d",&nextColor);
    board->next = (Color)nextColor;
    for(int i = 0;i<MAX_ROW;i++)
    {
        for(int j =0;j<MAX_COL;j++)
        {
            int typeNum, colorNum;
            fscanf(fp,"%d:%d[ ]",&colorNum,&typeNum);
            if(colorNum != 9)
            {
                bool exists = false;
                PieceType type = (PieceType)typeNum;
                Color color = (PieceType)colorNum;
                ChessPiece *moving = first;
                while(moving!=NULL && !exists)
                {
                    if(moving->type == type && moving->color == color)
                    {
                        exists = true;
                        board->square[i][j].Piece = moving;
                    }
                    moving = moving->next;
                }
                if(!exists)
                {
                    first = AppendPiece(first,type,color);
                    board->square[i][j].Piece = first;
                }
            }
        }
    }
    fclose(fp);
    board->first = first;

    return board;
}
