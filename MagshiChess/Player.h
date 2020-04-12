#pragma once

#include <string>
#include "Piece.h"
#include "Pipe.h"
#include "PipeInputOperations.h"
#include "King.h"

/* Player class, houses the board and king of the player */
class Player
{
public: 
	Player(Color c, std::string startingBoard, Color onlinePlayerColor);    //c'tor
	~Player();   //d'tor

	std::tuple<bool, Piece*> makeMove(std::tuple<int, int, int, int> positions); // gets a move and "makes" it  (changes the values in the board) also returns the eaten piece
	void undoMove(std::tuple<int, int, int, int>positions, std::tuple<bool, Piece*> eatenPiece); // gets a move and an eaten piece and undos that move
	King* getKing() const; // getter
	Color getColor() const; // getter
	Piece*** getBoard(); // getter
	error_level_code isValidCMD(std::tuple<int, int, int, int> positions); // returns if a move is valid, checks everything.
	bool isChecked(Piece*** board, std::tuple<int, int, int, int> positions); // getter

private:
	Piece*** _board;
	Color _color;
	King* _king;
};

