#ifndef DISPLAY_H
#define DISPLAY_H
#include "../Model/Entity.h"
void drawMenu(SDL_Renderer *renderer);
void drawGame(ChessBoard *cb,SDL_Renderer *renderer);
void drawLoadMenu(SDL_Renderer *renderer,bool loadedFile);
void showAvailableMoves(SDL_Renderer *renderer,const Moves move);
void drawReplay(ChessBoard *cb,SDL_Renderer *renderer);
#endif // DISPLAY_H

//void drawMenu(SDL_Renderer *renderer);
