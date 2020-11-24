#ifndef MOVES_H
#define MOVES_H
void freeMoves(Moves *list);
void addMove(Moves *list, Coordinate newValue);
void freeMoveHistory(Move *first);
Move *addMoveToHistory(Move *first,const Coordinate from,const Coordinate to);
void addMoveToMovement(Replay *list, Movement newValue);
#endif // MOVES_H
