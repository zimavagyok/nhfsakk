#include "Entity.h"

void freeMoves(Moves *list)
{
    free(list->coordinates);
}

void addMove(Moves *list, Coordinate newValue)
{
    list->moveCount+=1;
    Coordinate *newCoordinates = malloc(list->moveCount * sizeof(Coordinate));
    if(list->moveCount != 0)
    {
        for(int i = 0;i<list->moveCount-1;i++)
        {
            newCoordinates[i] = list->coordinates[i];
        }
    }
    free(list->coordinates);
    newCoordinates[list->moveCount-1] = newValue;
    list->coordinates = newCoordinates;
}
