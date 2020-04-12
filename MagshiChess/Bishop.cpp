#include "Bishop.h"
#include "PipeInputOperations.h"
#include <tuple>
#include <cmath>

Bishop::Bishop(Color color) : Piece(color){
	/*
	Setting the pieceChar according to the color of the piece
	*/
	switch (color) {
	case Color::white:
			this->_pieceChar = 'B';
			break;
	case Color::black:
			this->_pieceChar = 'b';
			break;
	}
}
Bishop::~Bishop() {

}
bool Bishop::validMove(Piece*** board, std::tuple<int, int, int, int> positions) {
	auto [srcRow, srcCol, dstRow, dstCol] = positions;
	/* Checking that there are no pieces in between the src and dst positons*/
	if (dstRow - srcRow == dstCol - srcCol and dstCol > srcCol and dstRow > srcRow ) { // Top Right Diagnoal check
		for (int i = srcRow + 1, j = srcCol + 1; i < dstRow; i++, j++) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol < srcCol and srcRow < dstRow) { // Top Left Diagnoal check
		for (int i = srcRow + 1, j = srcCol - 1; i < dstRow; i++, j--) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol < srcCol and srcRow > dstRow) { // Bottom Left Diagnoal check
		for (int i = srcRow - 1, j = srcCol - 1; i > dstRow; i--, j--) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	else if (abs(srcRow - dstRow) == abs(srcCol - dstCol) and dstCol > srcCol and srcRow > dstRow) { // Bottom Right Diagnoal check
		for (int i = srcRow - 1, j = srcCol + 1; i > dstRow; i--, j++) {
			if (board[i][j] != nullptr) {
				return false;
			}
		}
		return true;
	}
	return false;
}