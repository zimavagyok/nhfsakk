#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum Babu{ FUTO, KIRALY, HUSZAR, GYALOG, KIRALYNO, BASTYA} Babu;
typedef enum Szin { FEHER, FEKETE} Szin;
enum { MERET = 52 };
int karakterek = 32;

typedef struct Karakter{
    SDL_Texture *image;
    SDL_Rect image_position;
    Babu tipus;
    Szin szin;
} Karakter;

typedef struct Mezo{
    SDL_Rect mezoPoz;
    Karakter *babu;
} Mezo;

typedef struct elerhetoLepesek{
    int *indexek,darabszam;
} elerhetoLepesek;

typedef struct Lepes{
    int index;
    Mezo mezo;
} Lepes;

typedef struct Pos{
    int index, x,y;
} Pos;

void torolBabu(Karakter *babuTomb,Karakter *melyiket, int *karakterek)
{
    *karakterek -= 1;
    Karakter *ujBabuk = malloc(sizeof(Karakter)*(*karakterek));
    int index = 0;
    for(int i = 0;i<(*karakterek)+1;i++)
    {
        if(!SDL_RectEquals(&babuTomb->image_position,&melyiket->image_position))
        {
            ujBabuk[index++] = babuTomb[i];
        }
    }
    free(babuTomb);
    babuTomb = ujBabuk;
}

void lepes(Mezo *honnan, Mezo *hova, Karakter *babuTomb, int *karakterek, bool torol)
{
    if(hova->babu != NULL && torol)
    {
        torolBabu(babuTomb,hova->babu,karakterek);
    }
    printf("%d %d\n",honnan->mezoPoz.x, honnan->babu->tipus);
    hova->babu = honnan->babu;
    hova->babu->image_position = hova->mezoPoz;
    honnan->babu = NULL;
}

void lepesVissza(Lepes *elso, Lepes *masodik, Mezo *mezok)
{
    mezok[elso->index] = elso->mezo;
    mezok[masodik->index] = masodik->mezo;
}

void bovit(elerhetoLepesek *tomb, int elem)
{
    tomb->darabszam+=1;
    printf("%d\n",tomb->darabszam);
    int *ujTomb = malloc(tomb->darabszam * sizeof(int));
    if(tomb->indexek != NULL)
        ujTomb = tomb->indexek;
    ujTomb[tomb->darabszam - 1] = elem;
    free(tomb->indexek);
    tomb->indexek = ujTomb;
}
void kirajzolTabla(Mezo *mezok)
{
    printf("\n");
    for(int i = 0;i<8;i++)
    {
        for(int j = 0;j<8;j++)
        {
            if(mezok[i*8+j].babu == NULL)
            printf("%2d ", -1);
            else
            printf("%2d ",mezok[i*8+j].babu->tipus);
        }
        printf("\n");
    }
}

bool Sakk(Mezo *mezok,Szin szin)
{
    int kiralyIndex;
    for(int i = 0;i<64;i++)
    {
        if(mezok[i].babu != NULL && mezok[i].babu->tipus==KIRALY && mezok[i].babu->szin == szin)
        {
            kiralyIndex = i;
            break;
        }
    }
    int kiralySor = kiralyIndex % 8;
    int kiralyOszlop = kiralyIndex / 8;
    kirajzolTabla(mezok);

    int loX[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int loY[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    for(int i = 0;i<8;i++)
    {
        int oszlop = kiralyIndex % 8;
        int sor = kiralyIndex / 8;
        int index = (sor + loX[i]) * 8 + oszlop + loY[i];
        if(index >0 && index <64 && mezok[index].babu != NULL && mezok[index].babu->tipus == HUSZAR && mezok[index].babu->szin != szin)
        {
            return true;
        }
    }

    //HORIZONTÁLIS
    for(int i = kiralySor+1; i<8;i++)
    {
        int index = kiralyOszlop * 8 + i;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == BASTYA || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    for(int i = kiralySor-1; i>=0;i--)
    {
        int index = kiralyOszlop * 8 + i;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == BASTYA || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    //VERTIKÁLIS
    for(int i = kiralyOszlop+1; i<8;i++)
    {
        int index = i * 8 + kiralySor;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == BASTYA || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    for(int i = kiralyOszlop-1; i>=0;i--)
    {
        int index = i * 8 + kiralySor;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == BASTYA || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    //ÁTLÓS
    for(int i = kiralyOszlop+1, j = kiralySor+1; i<8 && j < 8;i++,j++)
    {
        int index = i * 8 + j;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == FUTO || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    for(int i = kiralyOszlop-1, j = kiralySor+1; i>=0 && j < 8;i--,j++)
    {
        int index = i * 8 + j;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == FUTO || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    for(int i = kiralyOszlop+1, j = kiralySor-1; i<8 && j >= 0;i++,j--)
    {
        int index = i * 8 + j;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == FUTO || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    for(int i = kiralyOszlop-1, j = kiralySor-1; i>=0 && j >= 0;i--,j--)
    {
        int index = i * 8 + j;
        if(mezok[index].babu!=NULL)
        {
            if(mezok[index].babu->szin == szin)
            {
                break;
            }
            else if(mezok[index].babu->tipus == FUTO || mezok[index].babu->tipus == KIRALYNO)
            {
                return true;
            }
        }
    }

    int gyalogX[] = {1,-1,1,-1};
    int gyalogY[] = {1,1,-1,-1};
    for(int i = 0;i<4;i++)
    {
        int index = (kiralySor+gyalogY[i]) * 8 + kiralyOszlop + gyalogX[i];
        if(index<64 && index >= 0 && mezok[index].babu != NULL && mezok[index].babu->tipus == GYALOG && mezok[index].babu->szin == FEKETE && szin == FEHER)
            return true;
        else if(index<64 && index >= 0 && mezok[index].babu != NULL && mezok[index].babu->tipus == GYALOG && mezok[index].babu->szin == FEHER && szin == FEKETE)
            return true;
    }

    return false;
}

elerhetoLepesek lephetoMezok(Mezo *mezok, Mezo *melyik, Szin szin, Karakter *babuTomb, int *karakterek)
{
    Pos valasztott = {.x = melyik->mezoPoz.x / MERET,.y=melyik->mezoPoz.y / MERET, .index = (melyik->mezoPoz.y / MERET) * 8 + melyik->mezoPoz.x / MERET};
    elerhetoLepesek lepesek;
    lepesek.darabszam = 0;
    lepesek.indexek = NULL;
    Lepes honnan, hova;
    if(melyik == NULL)
    {
        printf("nem");
    }

    switch(melyik->babu->tipus)
    {
        case GYALOG:
            if(melyik->babu->szin == FEHER)
            {
                if(valasztott.x-1 >= 0 && valasztott.y+1 >= 0 && mezok[(valasztott.y+1)*8+(valasztott.x-1)].babu->szin != melyik->babu->szin)
                {
                    int index = (valasztott.y+1)*8+(valasztott.x-1);
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(&melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                if(valasztott.x+1 <8  && valasztott.y+1 >= 0 && mezok[(valasztott.y+1)*8+(valasztott.x+1)].babu->szin != melyik->babu->szin)
                {
                    int index = (valasztott.y+1)*8+(valasztott.x+1);
                    honnan = (Lepes){.mezo=*melyik,.index=valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                if(valasztott.y+1 >= 0 && mezok[(valasztott.y+1)*8+valasztott.x].babu->szin != melyik->babu->szin)
                {
                    int index = (valasztott.y+1)*8+valasztott.x;
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
            }
            else
            {
                if(valasztott.x-1 >= 0 && valasztott.y-1 >= 0 && mezok[(valasztott.y+1)*8+(valasztott.x-1)].babu->szin != melyik->babu->szin)
                {
                    int index = (valasztott.y-1)*8+(valasztott.x-1);
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                if(valasztott.x+1 <8 && valasztott.y-1 >= 0 && mezok[(valasztott.y+1)*8+(valasztott.x+1)].babu->szin != melyik->babu->szin)
                {
                    int index = (valasztott.y-1)*8+(valasztott.x+1);
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                if(valasztott.y-1 >= 0 || mezok[(valasztott.y-1)*8+valasztott.x].babu == NULL)
                {
                    int index = (valasztott.y-1)*8+valasztott.x;
                    honnan = (Lepes){.mezo=*melyik,.index=valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    kirajzolTabla(mezok);
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    kirajzolTabla(mezok);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
            }
            break;
        case HUSZAR: ;
            int loX[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
            int loY[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
            for(int i = 0;i<8;i++)
            {
                int index = (valasztott.x + loX[i]) * 8 + valasztott.y + loY[i];
                if(index >0 && index <64 && mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
            }
            break;
        case BASTYA:
            for(int i = valasztott.x+1; i<8;i++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1; i>=0;i--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            for(int i = valasztott.x+1; i<8;i++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1; i>=0;i--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            break;
        case FUTO:
            //ÁTLÓS
            for(int i = valasztott.x+1, j = valasztott.y+1; i<8 && j < 8;i++,j++)
            {
               int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1, j = valasztott.y+1; i>=0 && j < 8;i--,j++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x+1, j = valasztott.y-1; i<8 && j >= 0;i++,j--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1, j = valasztott.y-1; i>=0 && j >= 0;i--,j--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            break;
        case KIRALYNO:
            for(int i = valasztott.x+1; i<8;i++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1; i>=0;i--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            for(int i = valasztott.x+1; i<8;i++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1; i>=0;i--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            //ÁTLÓS
            for(int i = valasztott.x+1, j = valasztott.y+1; i<8 && j < 8;i++,j++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1, j = valasztott.y+1; i>=0 && j < 8;i--,j++)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x+1, j = valasztott.y-1; i<8 && j >= 0;i++,j--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }

            for(int i = valasztott.x-1, j = valasztott.y-1; i>=0 && j >= 0;i--,j--)
            {
                int index = valasztott.y * 8 + i;
                if(mezok[index].babu->szin == szin)
                {
                    break;
                }
                else if(mezok[index].babu == NULL)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
                else if(mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                    break;
                }
            }
            break;
        case KIRALY:;
            int kiralyX[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
            int kiralyY[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
            for(int i = 0;i<8;i++)
            {
                int index = (valasztott.x + kiralyX[i]) * 8 + valasztott.y + kiralyY[i];
                if(index >0 && index <64 && mezok[index].babu->szin != szin)
                {
                    honnan = (Lepes){.mezo=*melyik,.index = valasztott.index};
                    hova = (Lepes){.mezo=mezok[index],.index = index};
                    lepes(melyik,&mezok[index],babuTomb,karakterek,false);
                    if(!Sakk(mezok,szin))
                    {
                        bovit(&lepesek,index);
                    }
                    lepesVissza(&honnan,&hova,mezok);
                }
            }
            break;
    }
    return lepesek;
}

void feltolt(Karakter *babukT, SDL_Renderer *renderer, Mezo *elemT)
{
    printf("%s","xddd");
    babukT[0].tipus = BASTYA;
    babukT[0].image = IMG_LoadTexture(renderer, "WRook.png");
    babukT[1].tipus = HUSZAR;
    babukT[1].image = IMG_LoadTexture(renderer, "WKnight.png");
    babukT[2].tipus = FUTO;
    babukT[2].image = IMG_LoadTexture(renderer, "WBishop.png");
    babukT[3].tipus = KIRALY;
    babukT[3].image = IMG_LoadTexture(renderer, "WKing.png");
    babukT[4].tipus = KIRALYNO;
    babukT[4].image = IMG_LoadTexture(renderer, "WQueen.png");
    babukT[5].tipus = FUTO;
    babukT[5].image = IMG_LoadTexture(renderer, "WBishop.png");
    babukT[6].tipus = HUSZAR;
    babukT[6].image = IMG_LoadTexture(renderer, "WKnight.png");
    babukT[7].tipus = BASTYA;
    babukT[7].image = IMG_LoadTexture(renderer, "WRook.png");

    for(int i = 0;i<8;i++)
    {
        babukT[i].szin = FEHER;
        babukT[i].image_position = (SDL_Rect){.x=i*MERET, .y=0, .w=MERET,.h=MERET};
        elemT[i].babu = &babukT[i];
        elemT[i].mezoPoz = babukT[i].image_position;
    }

    for(int i = 8;i<16;i++)
    {
        babukT[i].image = IMG_LoadTexture(renderer, "WPawn.png");
        babukT[i].tipus = GYALOG;
        babukT[i].szin = FEHER;
        babukT[i].image_position = (SDL_Rect){.x=(i%8)*MERET, .y=MERET, .w=MERET,.h=MERET};
        elemT[i].babu = &babukT[i];
        elemT[i].mezoPoz = babukT[i].image_position;
    }

    for(int i = 16;i<48;i++)
    {
        elemT[i].mezoPoz = (SDL_Rect){.x=(i%8)*MERET, .y=(i/8)*MERET, .w=MERET,.h=MERET};
        elemT[i].babu = NULL;
    }

    babukT[16].tipus = BASTYA;
    babukT[16].image = IMG_LoadTexture(renderer, "BRook.png");
    babukT[17].tipus = HUSZAR;
    babukT[17].image = IMG_LoadTexture(renderer, "BKnight.png");
    babukT[18].tipus = FUTO;
    babukT[18].image = IMG_LoadTexture(renderer, "BBishop.png");
    babukT[19].tipus = KIRALY;
    babukT[19].image = IMG_LoadTexture(renderer, "BKing.png");
    babukT[20].tipus = KIRALYNO;
    babukT[20].image = IMG_LoadTexture(renderer, "BQueen.png");
    babukT[21].tipus = FUTO;
    babukT[21].image = IMG_LoadTexture(renderer, "BBishop.png");
    babukT[22].tipus = HUSZAR;
    babukT[22].image = IMG_LoadTexture(renderer, "BKnight.png");
    babukT[23].tipus = BASTYA;
    babukT[23].image = IMG_LoadTexture(renderer, "BRook.png");

    int j = 48;
    for(int i = 24;i<32;i++)
    {
        babukT[i].image = IMG_LoadTexture(renderer, "BPawn.png");
        babukT[i].tipus = GYALOG;
        babukT[i].szin = FEKETE;
        babukT[i].image_position = (SDL_Rect){(i%8)*MERET, 6*MERET, MERET,MERET};
        elemT[j].mezoPoz = babukT[i].image_position;
        elemT[j].babu = &babukT[i];
        //printf("%d,%d\n",elemT[j].elemPoz.x,elemT[j].elemPoz.y);
        j++;
    }

    j=56;
    for(int i = 16;i<24;i++)
    {
        babukT[i].szin = FEKETE;
        babukT[i].image_position = (SDL_Rect){(i%8)*MERET, 7*MERET, MERET,MERET};
        elemT[j].mezoPoz = babukT[i].image_position;
        elemT[j].babu = &babukT[i];
        //printf("%d,%d: %d,%d\n",elemT[j].elemPoz.x,elemT[j].elemPoz.y,babukT[i].image_position.x,babukT[i].image_position.y);
        j++;
    }
}

void kirajzol(Karakter *elemek, SDL_Renderer *renderer, SDL_Texture *hatter)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,hatter,NULL,NULL);
    for(int i = 0;i<karakterek;i++)
        SDL_RenderCopy(renderer,elemek[i].image,NULL,&elemek[i].image_position);
    SDL_RenderPresent(renderer);
}

bool elerhetoLepesekKozott(elerhetoLepesek tomb, int index)
{
    for(int i = 0;i<tomb.darabszam;i++)
    {
        if(tomb.indexek[i]==index)
        {
            return true;
        }
    }
    return false;
}

/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

int main(int argc, char *argv[]) {
    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("SDL peldaprogram", 416, 416, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

/* Update screen for the first time and wait */
    SDL_Texture *hatter = IMG_LoadTexture(renderer, "chessBoard.png");
    SDL_RenderPresent(renderer);

    Mezo elemek[64];
    Karakter *babuk = malloc(sizeof(Karakter)*32);
    feltolt(babuk,renderer,elemek);
    kirajzol(babuk,renderer,hatter);
    Szin kovetkezo = FEKETE;

    bool drag = false;
    SDL_Rect *selected = NULL;
    SDL_Event ev;
    SDL_Point mousePos;
    SDL_Point clickOffset;
    SDL_Rect actualPos;
    Mezo *kivalasztott = NULL;
    while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
            switch(ev.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                for(int i =0;i<64;i++)
                {
                    if(SDL_PointInRect(&mousePos,&elemek[i].mezoPoz) && elemek[i].babu != NULL && elemek[i].babu->szin == kovetkezo)
                    {
                        kivalasztott = &elemek[i];
                        clickOffset.x = mousePos.x - elemek[i].mezoPoz.x;
                        clickOffset.y = mousePos.y - elemek[i].mezoPoz.y;
                        actualPos = elemek[i].mezoPoz;
                        drag = true;
                        break;
                    }
                }

                break;
            case SDL_MOUSEMOTION:
                mousePos.x = ev.motion.x;
                mousePos.y = ev.motion.y;
                if(drag == true && kivalasztott != NULL)
                {
                    kivalasztott->babu->image_position.x = mousePos.x - clickOffset.x;
                    kivalasztott->babu->image_position.y = mousePos.y - clickOffset.y;
                    /*SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer,hatter,NULL,NULL);
                    SDL_RenderCopy(renderer,babuk[0].image,NULL,&babuk[0].image_position);
                    SDL_RenderPresent(renderer);*/
                    kirajzol(babuk,renderer,hatter);

                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(drag == true && kivalasztott != NULL){
                    //SDL_Rect ujPoz = {.x =  MERET*((mousePos.x)/MERET), MERET*((mousePos.y)/MERET), .w = MERET, .h = MERET};
                    for(int i = 0;i<64;i++)
                    {
                        if(SDL_PointInRect(&mousePos,&elemek[i].mezoPoz))
                        {
                            elerhetoLepesek tomb = lephetoMezok(elemek,kivalasztott,kovetkezo,babuk,&karakterek);
                            kirajzolTabla(elemek);
                            if(elerhetoLepesekKozott(tomb,i) && !SDL_RectEquals(&elemek[i].mezoPoz,&kivalasztott->mezoPoz))
                            {
                                lepes(kivalasztott,&elemek[i],babuk,&karakterek,true);
                                if(kovetkezo==FEKETE)
                                {
                                    kovetkezo = FEHER;
                                }
                                else
                                {
                                    kovetkezo=FEKETE;
                                }
                                kirajzol(babuk,renderer,hatter);
                            }
                            else
                            {
                                kivalasztott->babu->image_position = kivalasztott->mezoPoz;
                                kirajzol(babuk,renderer,hatter);
                            }
                            kirajzolTabla(elemek);
                            break;
                        }
                    }
                    //selected->x = MERET*((mousePos.x)/MERET);
                    //selected->y = MERET*((mousePos.y)/MERET);
                    //printf("%d,%d\n",ujPoz.y, kivalasztott->image_position.y);
                drag = false;
                kivalasztott = NULL;
                }
                break;
            }
    }


    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
