#include "Queen.h"
Queen::Queen(Color color) : Piece(color)
{
	this->_rook = new Rook(color);
	this->_bishop = new Bishop(color);
	/* Setting the piece char */
	switch (color) {
	case Color::white:
		this->_pieceChar = 'Q';
		break;
	case Color::black:
		this->_pieceChar = 'q';
		break;
	}
}
Queen::~Queen() {
	/* deleting the object */
	delete this->_rook;
	delete this->_bishop;
}
bool Queen::validMove(Piece*** board, std::tuple<int, int, int, int> positions) {
	return (this->_bishop->validMove(board, positions) or this->_rook->validMove(board, positions)); 
}