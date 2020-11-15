#ifndef CHESS_H
#define CHESS_H
tempBoard *doMoveTemp(const Coordinate from, const Coordinate to, tempBoard *tb);
bool addIfNotCheck(const Coordinate from, const Coordinate to, const ChessBoard *cb, Moves *legal);
bool checkCheck(const Coordinate from, const Coordinate to, const ChessBoard *cb);
void LegalMoves(const Coordinate from,const ChessBoard *cb,Moves *move);
bool doMove(const Coordinate from,const Coordinate to, ChessBoard *cb);
bool checkCheckMate(const ChessBoard *cb);
#endif // CHESS_H
