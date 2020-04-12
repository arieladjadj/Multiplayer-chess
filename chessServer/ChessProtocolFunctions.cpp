#include "ChessProtocolFunctions.hpp"
#pragma warning(disable:4996)
#include <string>

/*
This function convert from one player positions on board to another - in the move/crown cmd (data*)
*/
void ChessProtocol::convertData(char* data)
{
	if (!isMoveCmd(data))
		convertCrownCmd(data);
	else
		convertMoveCmd(data);
}

/*
This function convert from one player positions on board to another - in the move cmd (data*)
*/
void ChessProtocol::convertMoveCmd(char* data)
{
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0;
	srcRow = data[5] - '0';
	srcCol = data[7] - '0';
	dstRow = data[9] - '0';
	dstCol = data[11] - '0';
	data[5] = (char)(7 - srcRow + '0');
	data[7] = (char)(7 - srcCol + '0');
	data[9] = (char)(7 - dstRow + '0');
	data[11] = (char)(7 - dstCol + '0');
	
}

/*
This function convert from one player positions on board to another - in the crown cmd (data*)
*/
void ChessProtocol::convertCrownCmd(char* data)
{ 
	int dstRow = 0, dstCol = 0;
	dstRow = data[6] - '0';
	dstCol = data[8] - '0';

	data[6] = (char)(7 - dstRow + '0');
	data[8] = (char)(7 - dstCol + '0');

}

/*
This function checks if data is move cmd
*/
bool ChessProtocol::isMoveCmd(char* data)
{
	return (std::string(data).find("move") != std::string::npos);
}

/*
This function checks if data is msg cmd
*/
bool ChessProtocol::isMsgCmd(char* data)
{
	return std::string(data).find("chat") != std::string::npos;
}