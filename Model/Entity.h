#include "SDL2/SDL.h"
#include <stdbool.h>
#ifndef ENTITY_H
#define ENTITY_H
typedef enum PieceType {PAWN,ROOK,KNIGHT,BISHOP,KING,QUEEN,NONE}PieceType;
typedef enum Color {WHITE,BLACK} Color;
typedef enum MoveType {Castling,Upgrade,Normal} MoveType;
enum {SIZE = 52, MAX_COL = 8, MAX_ROW = 8};

typedef struct ChessPiece{
    int File, Rank;
    bool alive;
    SDL_Texture *image;
    SDL_Rect image_position;
    PieceType type;
    Color color;
    struct ChessPiece *next;
} ChessPiece;

/*typedef struct ChessPieceList{
    ChessPiece *current,*next;
} ChessPieceList;*/
typedef struct Coordinate
{
    int Rank, File;
} Coordinate;

typedef struct Square{
    ChessPiece *Piece;
    SDL_Rect squarePos;
    int File, Rank;
} Square;

typedef struct ChessBoard{
    Square square[8][8];
    ChessPiece *first;
    Color next;
} ChessBoard;

typedef struct tempSquare{
    Color color;
    PieceType type;
    SDL_Rect squarePos;
    int Rank,File;
} tempSquare;

typedef struct tempBoard{
    tempSquare squares[8][8];
    Color next;
} tempBoard;

typedef struct Move{
    int File,Rank;
    Square toSquare;
} Move;

typedef struct Moves{
    int moveCount;
    Coordinate *coordinates;
} Moves;
#endif
