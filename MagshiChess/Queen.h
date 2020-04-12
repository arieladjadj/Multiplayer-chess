#pragma once
#include "Bishop.h"
#include "Rook.h"

class Queen : public Piece {
public:
	Queen(Color color);
	~Queen();
	bool validMove(Piece*** board, std::tuple<int, int, int, int> positions); // implementing virtual function
private:
	/* Creating Rook and Bishop objects */
	Rook* _rook;
	Bishop* _bishop;
};
