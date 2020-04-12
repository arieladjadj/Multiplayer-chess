#pragma once
#include <tuple>

namespace ChessProtocol
{
	bool isMoveCmd(char* data);       // //This function checks if data is move cmd
	void convertMoveCmd(char* data); //This function convert from one player positions on board to another - in the move cmd (data*)
	bool isMsgCmd(char* data);      //This function checks if data is msg cmd
	void convertCrownCmd(char *data);
	void convertMoveCmd(char * data); 
	void convertData(char* data); //This function convert from one player positions on board to another - in the move/crown cmd(data*)
}
