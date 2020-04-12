#pragma once
#include "Piece.h"

Piece::Piece(Color color) {
	this->_color = color;
}
Color Piece::getColor() const{
	return this->_color; // return the color
}
Piece::~Piece() {

}
char Piece::getPieceChar() const {
	return this->_pieceChar; // return the piece char
}