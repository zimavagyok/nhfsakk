#include "../Model/Entity.h"
#include "../Model/Moves.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../debugmalloc.h"

void reversePrint(FILE *fp,Move *element)
{
    if(element == NULL)
    {
        return;
    }

    reversePrint(fp,element->nextMove);
    fprintf(fp,"%c%d::%c%d\n",'a'+element->from.File,element->from.Rank,'a'+element->to.File,element->to.Rank);
}

void WriteToFile(Move *first, ChessBoard *cb, bool ended)
{
    //get current time
    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    char * fileName = malloc(strlen(time_str)*sizeof(char) + 9*sizeof(char) + sizeof(char));
    int index = 0;
    for(int i = 0;i<strlen(time_str);i++)
    {
        if(time_str[i] == ' ' || time_str[i] == ':')
        {
            fileName[index++] = '_';
        }
        else
        {
            fileName[index++] = time_str[i];
        }
    }
    fileName[index++] = '.';
    fileName[index++] = 't';
    fileName[index++] = 'x';
    fileName[index++] = 't';
    fileName[index++] = '\0';

    FILE *fp;
    fp = fopen(fileName,"w");
    if(fp == NULL)
    {
        return;
    }

    fprintf(fp, ended ? "Chess:Replay\n" : "Chess:Save\n");
    if(ended)
    {
        reversePrint(fp,first);
    }
    else
    {
        fprintf(fp,"%d\n",cb->next);
        for(int i = 0;i<MAX_ROW;i++)
        {
            for(int j = 0;j<MAX_COL;j++)
            {
                if(cb->square[i][j].Piece!=NULL)
                    fprintf(fp,"%d:%d ",cb->square[i][j].Piece->color,cb->square[i][j].Piece->type);
                else
                    fprintf(fp,"%d:%d ",9,9);
            }
            fprintf(fp,"\n");
        }

    }
    free(fileName);
    fclose(fp);
    freeMoveHistory(first);
}

int loadMovesFromFile(Replay *arr,char *filePath)
{
    FILE *fp = fopen(filePath,"r");
    if(fp == NULL)
    {
        return 1;
    }
    char header[13];
    fscanf(fp,"%s",header);
    if(strcmp(header,"Chess:Replay")!=0)
    {
        if(fp!=NULL)
            fclose(fp);
        return 1;
    }

    Coordinate from,to;
    int fRank=0, tRank=0;
    char fFile='a', tFile='a', BUFF[100];
    while(fscanf(fp,"%s",BUFF)!= EOF)
    {
        sscanf(BUFF,"%c%d::%c%d",&fFile,&fRank,&tFile,&tRank);
        from= (Coordinate){.Rank = fRank, .File = fFile-'a'};
        to= (Coordinate){.Rank = tRank, .File = tFile-'a'};
        Movement newV = {.from = from, .to = to};
        addMoveToMovement(arr,newV);
    }
    fclose(fp);
    return 0;
}
