#include "SDL2/SDL.h"
#include <stdbool.h>
#ifndef ENTITY_H
#define ENTITY_H
typedef enum PieceType {PAWN,ROOK,KNIGHT,BISHOP,KING,QUEEN,NONE}PieceType;
typedef enum Color {WHITE,BLACK} Color;
enum {SIZE = 52, MAX_COL = 8, MAX_ROW = 8};

typedef struct ChessPiece{
    SDL_Texture *image;
    PieceType type;
    Color color;
    struct ChessPiece *next;
} ChessPiece;

/*typedef struct ChessPieceList{
    ChessPiece *current,*next;
} ChessPieceList;*/
typedef struct Text{
    int x,y,fontSize;
    SDL_Color color;
} Text;

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
    SDL_Texture *board;
    Square square[8][8];
    ChessPiece *first;
    Color next;
    bool loaded;
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
    Coordinate from, to;
    struct Move *nextMove;
} Move;

typedef struct Movement{
    Coordinate from,to;
} Movement;

typedef struct Replay{
    int movementCount;
    Movement *movement;
} Replay;

typedef struct Moves{
    int moveCount;
    Coordinate *coordinates;
} Moves;
#endif
