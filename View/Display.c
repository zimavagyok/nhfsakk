#include "../Init/Initialize.h"
#include "../Model/Entity.h"
#include <SDL2/SDL_ttf.h>

void drawGame(ChessBoard *cb,SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    SDL_Texture *boardPic = IMG_LoadTexture(renderer,"assets/images/chessBoard.png");
    SDL_Rect boardPos = {0,0,416,416};
    SDL_RenderCopy(renderer,boardPic,NULL,&boardPos);
    ChessPiece *moving = cb->first;
    while(moving!=NULL)
    {
        SDL_RenderCopy(renderer,moving->image,NULL,&moving->image_position);
        moving = moving->next;
    }
    cb->next == WHITE ? drawText("White moves!",renderer) : drawText("Black moves!",renderer);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(boardPic);
}

void drawText(char *text,SDL_Renderer *renderer)
{
    SDL_Color whiteText = {255, 255, 255};
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/fonts/alba.ttf", 32);
    SDL_Surface *sign;
    SDL_Texture *sign_t;
    SDL_Rect to = { 0, 0, 0, 0 };
    sign = TTF_RenderUTF8_Blended(font, text, whiteText);
    sign_t = SDL_CreateTextureFromSurface(renderer, sign);
    to.x = (400 - sign->w);
    to.y = 430;
    to.w = sign->w;
    to.h = sign->h;
    SDL_RenderCopy(renderer, sign_t, NULL, &to);
    SDL_FreeSurface(sign);
    SDL_DestroyTexture(sign_t);
    TTF_CloseFont(font);
}
