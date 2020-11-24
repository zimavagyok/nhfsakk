#include "Entity.h"
#include "../debugmalloc.h"

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

void addMoveToMovement(Replay *list, Movement newValue)
{
    list->movementCount+=1;
    Movement *newCoordinates = malloc(list->movementCount * sizeof(Movement));
    if(list->movementCount != 0)
    {
        for(int i = 0;i<list->movementCount-1;i++)
        {
            newCoordinates[i] = list->movement[i];
        }
    }
    free(list->movement);
    newCoordinates[list->movementCount-1] = newValue;
    list->movement = newCoordinates;
}

void freeMovement(Replay *element)
{
    free(element->movement);
}

void freeMoveHistory(Move *first)
{
    Move *current = first, *next;
    while(current!=NULL)
    {
        next = current->nextMove;
        free(current);
        current = next;
    }
}

Move *addMoveToHistory(Move *first,const Coordinate from,const Coordinate to)
{
    Move *newMove = malloc(sizeof(Move));
    newMove->from = from;
    newMove->to = to;
    newMove->nextMove = first;
    first = newMove;
    return first;
}
