#include "../Model/Entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../debugmalloc.h"

void drawText(char *text,SDL_Renderer *renderer,Text options)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("assets/fonts/alba.ttf", options.fontSize);
    SDL_Surface *sign;
    SDL_Texture *sign_t;
    SDL_Rect to = { 0, 0, 0, 0 };
    sign = TTF_RenderUTF8_Blended(font, text, options.color);
    sign_t = SDL_CreateTextureFromSurface(renderer, sign);
    to.x = (options.x - sign->w);
    to.y = options.y;
    to.w = sign->w;
    to.h = sign->h;
    SDL_RenderCopy(renderer, sign_t, NULL, &to);
    SDL_FreeSurface(sign);
    SDL_DestroyTexture(sign_t);
    TTF_CloseFont(font);
}

void drawMenu(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 500; ++i) //infoc-n talalt ciklus
        filledCircleRGBA(renderer, rand() % 416, rand() % 120,
                         10 + rand() % 5, rand() % 256, rand() % 256, rand() % 256, 64);

    Text textOpt = {.x=270,.y=30,.fontSize = 50, .color={255,255,255,0} };
    drawText("Chess",renderer,textOpt);
    SDL_RenderPresent(renderer);

    rectangleRGBA(renderer,130,130,286,180,255,255,255,255);
    textOpt = (Text){.x=240,.y=135,.fontSize = 30, .color={255,255,255,0} };
    drawText("Play",renderer,textOpt);

    rectangleRGBA(renderer,130,200,286,250,255,255,255,255);
    textOpt = (Text){.x=283,.y=205,.fontSize = 30, .color={255,255,255,0} };
    drawText("Load Game",renderer,textOpt);

    rectangleRGBA(renderer,130,270,286,320,255,255,255,255);
    textOpt = (Text){.x=240,.y=275,.fontSize = 30, .color={255,255,255,0} };
    drawText("Exit",renderer,textOpt);
    SDL_RenderPresent(renderer);
}

void drawLoadMenu(SDL_Renderer *renderer,bool loadedFile)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < 500; ++i) //infoc-n talalt ciklus
        filledCircleRGBA(renderer, rand() % 416, rand() % 120,
                         10 + rand() % 5, rand() % 256, rand() % 256, rand() % 256, 64);

    Text textOpt = {.x=loadedFile? 350 : 390,.y=30,.fontSize = 20, .color={255,255,255,0} };
    drawText(loadedFile?"Select one of the modes below!":"Please drop a txt file into the window",renderer,textOpt);

    rectangleRGBA(renderer,130,130,286,180,255,255,255,255);
    textOpt = (Text){.x=280,.y=140,.fontSize = 20, .color={255,255,255,0} };
    drawText("Continue a game",renderer,textOpt);

    rectangleRGBA(renderer,130,200,286,250,255,255,255,255);
    textOpt = (Text){.x=240,.y=210,.fontSize = 20, .color={255,255,255,0} };
    drawText("Replay",renderer,textOpt);

    rectangleRGBA(renderer,130,270,286,320,255,255,255,255);
    textOpt = (Text){.x=235,.y=280,.fontSize = 20, .color={255,255,255,0} };
    drawText("Back",renderer,textOpt);
    SDL_RenderPresent(renderer);
}

void drawGame(ChessBoard *cb,SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect boardPos = {0,0,416,416};
    SDL_RenderCopy(renderer,cb->board,NULL,&boardPos);
    for(int i = 0;i<MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            if(cb->square[i][j].Piece != NULL)
            {
                SDL_RenderCopy(renderer,cb->square[i][j].Piece->image,NULL,&cb->square[i][j].squarePos);
            }
        }
    }
    Text nextMove = {.x = 400, .y=446, .fontSize = 28, .color={255,255,255,0} };
    cb->next == WHITE ? drawText("White moves!",renderer,nextMove) : drawText("Black moves!",renderer,nextMove);

    Text textOpt = (Text){.x=70,.y=446,.fontSize = 28, .color={255,255,255,0} };
    drawText("Exit",renderer,textOpt);

    SDL_RenderPresent(renderer);
}

void showAvailableMoves(SDL_Renderer *renderer,const Moves move)
{
    for(int i = 0;i<move.moveCount;i++)
    {
        filledCircleRGBA(renderer,(move.coordinates[i].File * SIZE) + SIZE/2,(move.coordinates[i].Rank * SIZE) + SIZE/2,SIZE/6,0,0,0,100);
    }
    SDL_RenderPresent(renderer);
}

void drawReplay(ChessBoard *cb,SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect boardPos = {0,0,416,416};
    SDL_RenderCopy(renderer,cb->board,NULL,&boardPos);
    for(int i = 0;i<MAX_ROW;i++)
    {
        for(int j = 0;j<MAX_COL;j++)
        {
            if(cb->square[i][j].Piece != NULL)
            {
                SDL_RenderCopy(renderer,cb->square[i][j].Piece->image,NULL,&cb->square[i][j].squarePos);
            }
        }
    }
    Text textOpt = {.x = 400, .y=446, .fontSize = 25, .color={255,255,255,0} };
    drawText("Next",renderer,textOpt);
    textOpt = (Text) {.x = 330, .y=446,.fontSize = 25, .color={255,255,255,0} };
    drawText("Again",renderer,textOpt);

    textOpt = (Text){.x=70,.y=446,.fontSize = 25, .color={255,255,255,0} };
    drawText("Exit",renderer,textOpt);

    SDL_RenderPresent(renderer);
}


