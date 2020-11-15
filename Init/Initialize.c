#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "../main.h"
#include "../Model/Entity.h"

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
            //board->square[i][j] = (Square*)malloc(sizeof(Square));
            board->square[i][j].Rank = i;
            board->square[i][j].File = j;
            board->square[i][j].Piece = NULL;
            board->square[i][j].squarePos = initPosByFileRank(i,j);
        }
    }

    ChessPiece *first = NULL;

    //WHITE
    for(int j = 0;j<8;j++)
    {
        first = AppendPiece(first,1,j,PAWN,WHITE,"assets/images/WPawn.png");
        board->square[1][j].Piece = first;
    }

    first=AppendPiece(first,0,0,ROOK,WHITE,"assets/images/WRook.png");
    board->square[0][0].Piece = first;
    first=AppendPiece(first,0,1,KNIGHT,WHITE,"assets/images/WKnight.png");
    board->square[0][1].Piece = first;
    first=AppendPiece(first,0,2,BISHOP,WHITE,"assets/images/WBishop.png");
    board->square[0][2].Piece = first;
    first=AppendPiece(first,0,3,KING,WHITE,"assets/images/WKing.png");
    board->square[0][3].Piece = first;
    first=AppendPiece(first,0,4,QUEEN,WHITE,"assets/images/WQueen.png");
    board->square[0][4].Piece = first;
    first=AppendPiece(first,0,7,ROOK,WHITE,"assets/images/WRook.png");
    board->square[0][7].Piece = first;
    first=AppendPiece(first,0,6,KNIGHT,WHITE,"assets/images/WKnight.png");
    board->square[0][6].Piece = first;
    first=AppendPiece(first,0,5,BISHOP,WHITE,"assets/images/WBishop.png");
    board->square[0][5].Piece = first;

    //BLACK
    for(int j = 0;j<8;j++)
    {
        first = AppendPiece(first,6,j,PAWN,BLACK,"assets/images/BPawn.png");
        board->square[6][j].Piece = first;
    }

    first=AppendPiece(first,7,0,ROOK,BLACK,"assets/images/BRook.png");
    board->square[7][0].Piece = first;
    first=AppendPiece(first,7,1,KNIGHT,BLACK,"assets/images/BKnight.png");
    board->square[7][1].Piece = first;
    first=AppendPiece(first,7,2,BISHOP,BLACK,"assets/images/BBishop.png");
    board->square[7][2].Piece = first;
    first=AppendPiece(first,7,3,KING,BLACK,"assets/images/BKing.png");
    board->square[7][3].Piece = first;
    first=AppendPiece(first,7,4,QUEEN,BLACK,"assets/images/BQueen.png");
    board->square[7][4].Piece = first;
    first=AppendPiece(first,7,7,ROOK,BLACK,"assets/images/BRook.png");
    board->square[7][7].Piece = first;
    first=AppendPiece(first,7,6,KNIGHT,BLACK,"assets/images/BKnight.png");
    board->square[7][6].Piece = first;
    first=AppendPiece(first,7,5,BISHOP,BLACK,"assets/images/BBishop.png");
    board->square[7][5].Piece = first;

    board->first = first;
    board->next = WHITE;

    return board;
}
