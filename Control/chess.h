#ifndef CHESS_H
#define CHESS_H
bool doMove(const Coordinate from,const Coordinate to, ChessBoard *cb);
bool checkCheckMate(const ChessBoard *cb);
void doMoveReplay(const Movement replayMove, ChessBoard *cb);
#endif // CHESS_H
