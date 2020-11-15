#ifndef ChessPieceList_H
#define ChessPieceList_H
ChessPiece *AppendPiece(ChessPiece *first,const int Rank,const int File,const PieceType type,const Color color,const char *fileName);
void FreePieces(ChessPiece *First);
ChessPiece *RemovePiece(ChessPiece *First, ChessPiece *which);
#endif // ChessPieceList_H
