#pragma once
#include "Piece.h"

class Knight : public Piece {
public:
	Knight(Color color);
	~Knight();
	bool validMove(Piece*** board, std::tuple<int, int, int, int> positions); // implementing virtual function
};
