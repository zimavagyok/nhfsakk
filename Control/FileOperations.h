#ifndef FileOp_H
#define FileOp_H

void WriteToFile(Move *first,ChessBoard *cb, bool ended);
int loadMovesFromFile(Replay *arr,char *filePath);
#endif // FileOp_H
