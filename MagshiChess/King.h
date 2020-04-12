#pragma once
#include "Piece.h"
#include <string>
#include <tuple>

#define MOVING_RANGE 1
class King : public Piece
{
public:
	King(Color color, std::string& startingBoard, Color onlinePlayerColor);
	~King();

	virtual bool validMove(Piece*** board, std::tuple<int, int, int, int> positions) ; // implementing virtual function
	virtual bool isChecked(Piece*** board) ; // returns if the king is checked or not

	void setPos(int* new_pos); // setter
	int* getPos() ; // getter

private:
	bool _moved;
	int* _pos; //pos[0] = row, pos[1] = col   
};