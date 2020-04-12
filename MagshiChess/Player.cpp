#include "Player.h"
#include <locale>


Player::Player(Color c, std::string startingBoard,Color onlinePlayerColor) //c'tor
{
	this->_color = c;
	this->_board = PipeInputOperations::generateBoard(this->_color, startingBoard, onlinePlayerColor);
	/*
	Creating the king object
	*/
	if (onlinePlayerColor == Color::white)
		if(c == Color::black) 
			this->_king = dynamic_cast<King*>((this->_board[7][4]));  //set king piece of player
		else 
			this->_king = dynamic_cast<King*>(this->_board[0][4]); 
	else
			if(c == Color::black) 
				this->_king = dynamic_cast<King*>((this->_board[0][3]));
			else  this->_king = dynamic_cast<King*>((this->_board[7][3]));
}

Player::~Player()  //d'tor
{
	//TODO: Call the d'tor of the class that communicate with the server
	delete[] this->_board;
}

King* Player::getKing() const // getter
{
	return this->_king;
}

Piece*** Player::getBoard() // getter
{
	return this->_board;
}
std::tuple<bool, Piece*> Player::makeMove(std::tuple<int,int,int,int>positions)
{
	std::tuple<bool, Piece*> eatenPiece;
	auto [srcRow, srcCol, dstRow, dstCol] = positions; // gets the src and dst positons

	if (dynamic_cast<King*>(this->_board[srcRow][srcCol]) != nullptr) // if the move is moving the king
	{
		/* Updating the new king loctation */
		King* temp = dynamic_cast<King*>(this->_board[srcRow][srcCol]);
		int* new_pos = new int[2];
		new_pos[0] = dstRow;
		new_pos[1] = dstCol;
		temp->setPos(new_pos);
		delete[] new_pos;
	}

	if (this->_board[dstRow][dstCol] == nullptr) // if we the move didnt eat any piece
		eatenPiece = std::make_tuple(false, nullptr);
	else eatenPiece = std::make_tuple(true, this->_board[dstRow][dstCol]);

	Piece* temp = this->_board[srcRow][srcCol];
	this->_board[dstRow][dstCol] = temp;
	this->_board[srcRow][srcCol] = nullptr;


	return eatenPiece;
}

void Player::undoMove(std::tuple<int, int, int, int>positions, std::tuple<bool, Piece*> eatenPiece)
{
	auto [srcRow, srcCol, dstRow, dstCol] = positions; // getting the src and dst positions
	auto [isEaten, eatenPiecePtr] = eatenPiece; // getting the eaten piece

	Piece* temp1 = this->_board[dstRow][dstCol];
	this->_board[srcRow][srcCol] = temp1;
	if (!isEaten) // the move didnt eat a piece
		this->_board[dstRow][dstCol] = nullptr;
	else this->_board[dstRow][dstCol] = eatenPiecePtr;

	if (dynamic_cast<King*>(this->_board[srcRow][srcCol]) != nullptr) // if we are undoing a move that moved the king
	{
		/* Updating the king location */
		King* temp = dynamic_cast<King*>(this->_board[srcRow][srcCol]);
		int* new_pos = new int[2];
		new_pos[0] = srcRow;
		new_pos[1] = srcCol;
		temp->setPos(new_pos);
	}
}

Color Player::getColor() const // getter 
{
	return this->_color;
}
error_level_code Player::isValidCMD(std::tuple<int, int, int, int> positions) 
{
	auto [srcRow, srcCol, dstRow, dstCol] = positions; // getting the src and dst positions
	error_level_code response_code = valid;

	//check error_level_code 2 - in src position there is no piece of curr player
	if (this->_board[srcRow][srcCol] == nullptr || this->_board[srcRow][srcCol]->getColor() != this->_color)
	{
		response_code = invalid_src_not_occupied;
	}
	//check error_level_code 3 - in dst position there is piece of curr player
	else if (this->_board[dstRow][dstCol] != nullptr && this->_board[dstRow][dstCol]->getColor() == this->_color)
	{
		response_code = invalid_dst_occupied;
	} 
	else if (srcRow == dstRow && srcCol == dstCol)  //check error_level_vode 7 - dst posiiton and src position are same
	{
		response_code = invlaid_same_src_dst;
	}
	//check error_level_code 6 - invalid move of piece
	else if (this->_board[srcRow][srcCol] != nullptr && !this->_board[srcRow][srcCol]->validMove(this->_board, positions))
	{
		response_code = invalid_behevior; 
	}
	else   
	{
		std::tuple<bool, Piece*> eatenPiece;
		eatenPiece = this->makeMove(positions);
		
		if (!this->_king->isChecked(this->_board))
		{
			this->undoMove(positions, eatenPiece);
			return valid;
		}
		else
		{
			this->undoMove(positions, eatenPiece);
			return invalid_make_check;
		}
	}
	return response_code;
}

bool Player::isChecked(Piece*** board, std::tuple<int, int, int, int> positions)
{
	auto [srcRow, srcCol, dstRow, dstCol] = positions;
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (board[row][col] != nullptr && board[row][col]->getColor() != this->_color)
			{
				if (board[row][col]->validMove(board, std::make_tuple(row, col, dstRow, dstCol)))  //some piece can eat the king
					return true;
			}
		}
	}
	return false;
}


