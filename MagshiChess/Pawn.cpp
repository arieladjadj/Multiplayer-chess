#include "Pawn.h"
#include "PipeInputOperations.h"
#include <cmath>        // std::abs

Pawn::Pawn(Color c, Color onlinePlayerColor) : Piece(c)
{
	this->_moved = false;
	this->_onlinePlayerColor = onlinePlayerColor;
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (c) {
	case Color::white:
		this->_pieceChar = 'P';
		break;
	case Color::black:
		this->_pieceChar = 'p';
		break;
	}
}

Pawn::~Pawn()
{
}

bool Pawn::validMove(Piece*** board,std::tuple<int,int,int,int> positions) 
{
	auto [srcRow, srcCol, dstRow, dstCol] = positions; // getting source and dst position
	int distance = 0;

	if (this->_onlinePlayerColor == Color::white)
	{
		if (this->_color == Color::white)
			distance = dstRow - srcRow;

		else
			distance = srcRow - dstRow;
	}
	else
	{
		if (this->_color == Color::white)
			distance = srcRow - dstRow;

		else
			distance = dstRow - srcRow;
	}

	if (board[dstRow][dstCol] != nullptr && board[dstRow][dstCol]->getColor() != this->_color)  //when eating enemy piece
	{			
		if (distance != MOVED_PAWN_MOVING_RANGE)
			return false;
		
		else{
			if (std::abs(dstCol - srcCol) == PAWN_EATING_MOVE_RANGE) {
				if (!this->_moved) this->_moved = true;  //update bool var _moved
				return true;
			}
			else return false;

		}

	}
	else
	{
		if (srcCol != dstCol)
			return false;
		else
		{
			if (this->_moved) //piece moved already
			{

				if (distance != MOVED_PAWN_MOVING_RANGE)
					return false;
				else
					return true;
			}
			else  //first step
			{
				if (!(distance == MOVED_PAWN_MOVING_RANGE || distance == UNMOVED_PAWN_MOVING_RANGE))
					return false;
				else
				{
					this->_moved = true;
					return true;
				}
			}
		}
	}
}