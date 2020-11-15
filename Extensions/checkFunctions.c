#include "../Model/Entity.h"

bool insideBoarders(const Coordinate nextCoord)
{
    if(nextCoord.Rank < 0 || nextCoord.Rank >= MAX_ROW || nextCoord.File < 0 || nextCoord.File>=MAX_COL)
    {
        return false;
    }
    return true;
}

bool tempSquareEmpty(const Coordinate coord,const tempBoard *board)
{
    return board->squares[coord.Rank][coord.File].type == NONE ? true : false;
}

bool squareEmpty(const Coordinate coord,const ChessBoard *board)
{
    return board->square[coord.Rank][coord.File].Piece == NULL ? true : false;
}

bool compareCoordinates(const Coordinate first,const Coordinate second)
{
    return first.Rank == second.Rank && first.File == second.File ? true : false;
}

