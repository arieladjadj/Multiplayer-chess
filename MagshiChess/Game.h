#pragma once
#include "Player.h"
#include <SFML/Network.hpp>
#include <mutex>

class Game
{
public:
	Game(std::string& startingBoard, Pipe &p, Pipe& change, std::mutex* mu, sf::TcpSocket* sock, Color onlinePlayerColor);
	~Game();
	void setCurrTurn(bool turn); // setter
	Player* getOtherPlayer() const; // getter
	Player* getOnlinePlayer() const; // getter

	void playTurn(std::string& getMessageFromGraphics); 

private:
	bool isValidCode(error_level_code e) const;
	Pipe _p;
	Pipe _change;
	bool _thisTurn;
	Color _onlinePlayerColor;
	Player*  _whitePlayer;
	Player *_blackPlayer;
	std::mutex* _mu;
	sf::TcpSocket* _sock;
};

