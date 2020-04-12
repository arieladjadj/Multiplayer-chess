#pragma once
#include <tuple>
#include "Piece.h"
#include <string>
#include "Rook.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include <locale>
 
/*
Static class functions for dealing with Input from the pipes
*/
class PipeInputOperations {
public:
	static std::tuple<int, int, int, int> moveToPos(std::string move); // converts move text into an int tuple housing the src and dst positions.
	static Piece*** generateBoard(Color& playerColor, std::string& startingBoard, Color onlinePlayerColor); // generate board array from a give board string
	static Piece* getPieceFromChar(char ch); // returns a piece from a given char
};