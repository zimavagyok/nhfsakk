#include "SDL2/SDL.h"
#include "../Model/Entity.h"
#ifndef INIT_H
#define INIT_H
void sdl_initialize();
SDL_Rect initPosByFileRank(int Rank, int File);
ChessBoard *initChessBoardFromFile(char *file);
ChessBoard *initChessBoard();
#endif
