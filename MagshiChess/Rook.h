#pragma once
#include "Piece.h"

class Rook : public Piece {
public:
	Rook(Color color);
	~Rook();
	bool validMove(Piece*** board, std::tuple<int, int, int, int> positions); // implementing virtual function
};