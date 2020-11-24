#ifndef ChessPieceList_H
#define ChessPieceList_H
void UpgradePawn(ChessBoard *cb, const Coordinate where);
ChessPiece *AppendPiece(ChessPiece *first,const int Rank,const int File,const PieceType type,const Color color);
void FreePieces(ChessPiece *First);
ChessPiece *RemovePiece(ChessPiece *First, ChessPiece *which);
#endif // ChessPieceList_H
