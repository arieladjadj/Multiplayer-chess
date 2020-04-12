#pragma once
#include "PipeInputOperations.h"

std::tuple<int, int, int, int> PipeInputOperations::moveToPos(std::string move) {
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	/*
	Convert the chars to int
	*/
	srcRow = move[1] - '0' - 1;
	dstRow = move[3] - '0' - 1;
	srcCol = move[0] - 'a';
	dstCol = move[2] - 'a';
	return std::make_tuple(srcRow, srcCol, dstRow, dstCol); // create tuple housing the src and dst positions
}


Piece*** PipeInputOperations::generateBoard(Color& playerColor, std::string& startingBoard, Color onlinePlayerColor)
{
	Piece*** temp = new Piece * *[8];
	Color piece_color;
	char ch;

	/* Allocate the array */
	for (int i = 0; i < 8; i++)
	{
		temp[i] = new Piece * [8];
	}

	for (int i = 7; i  >=0; i--)
	{
		for (int j = 7; j >= 0; j--)
		{
			ch = startingBoard.at(i*8 +j);
			if (isupper(ch))
				piece_color = Color::white;
			else
				piece_color = Color::black;

			/* Create a new piece according to the char */
			switch (ch)
			{
			case 'r':
			case 'R':
				temp[i][j] = new Rook(piece_color);
				break;
			case 'p':
			case 'P':
				temp[i][j] = new Pawn(piece_color, onlinePlayerColor);
				break;
			case 'b':
			case 'B':
				temp[i][j] = new Bishop(piece_color);
				break;
			case 'q':
			case 'Q':
				temp[i][j] = new Queen(piece_color);
				break;
			case 'K':
			case 'k':
				temp[i][j] = new King(piece_color, startingBoard, onlinePlayerColor);
				break;
			case 'N':
			case 'n':
				temp[i][j] = new Knight(piece_color);
				break;
			case '#':
				temp[i][j] = nullptr;
				break;
			default:
				break;
			}
		}
	}

	return temp;

}

Piece* PipeInputOperations::getPieceFromChar(char ch) {
	/* Create a new Piece according to the char */
	/* Used for the crowning */
	switch (ch) {
		case 'Q':
			return new Queen(Color::white);
			break;
		case 'q':
			return new Queen(Color::black);
			break;
		case 'B':
			return new Bishop(Color::white);
			break;
		case 'b':
			return new Bishop(Color::black);
			break;
		case 'N':
			return new Knight(Color::white);
			break;
		case 'n':
			return new Knight(Color::black);
			break;
		case '#':
			return nullptr;
			break;
	}
}
