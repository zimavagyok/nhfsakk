#ifndef ChessPieceList_H
#define ChessPieceList_H
void UpgradePawn(ChessBoard *cb, const Coordinate where);
ChessPiece *AppendPiece(ChessPiece *first,const PieceType type,const Color color);
void FreePieces(ChessPiece *First);
#endif // ChessPieceList_H
