#pragma once
#include <tuple>

enum class Color {white, black};
enum error_level_code { valid = 0, valid_check, invalid_src_not_occupied, invalid_dst_occupied, invalid_make_check, invalid_index_error, invalid_behevior, invlaid_same_src_dst, check_mate };



/* Pure virtual abstract class */
class Piece {
public:
	Piece(Color color); // C'TOR
	virtual ~Piece(); // virtual D'TOR
	Piece() {};

	virtual bool validMove(Piece*** board,std::tuple<int, int, int, int>positions) = 0;  //pure virtual function
	Color getColor() const; // getter
	char getPieceChar() const; // getter

protected:
	Color _color;
	char _pieceChar;
};