#pragma once
#include <SFML/Network.hpp>
#include <future>
#include <thread>
#include <vector>
#include <exception>

namespace ServerFunctions
{
	void sendDisconnectMsg(sf::TcpSocket* sock);
	int sendBoards(sf::TcpSocket* sock1, sf::TcpSocket* sock2);
	void sendDisconnectMsg(sf::TcpSocket* sock);
	void forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2);
	void matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2);
	void createNewConnection(sf::TcpListener& listener, sf::TcpSocket* client1, sf::TcpSocket* client2);
}

