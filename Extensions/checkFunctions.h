#ifndef CheckFunction_H
#define CheckFunction_H
bool insideBoarders(const Coordinate nextCoord);
bool tempSquareEmpty(const Coordinate coord,const tempBoard *board);
bool squareEmpty(const Coordinate coord,const ChessBoard *board);
bool compareCoordinates(const Coordinate first,const Coordinate second);
#endif // CheckFunction_H
