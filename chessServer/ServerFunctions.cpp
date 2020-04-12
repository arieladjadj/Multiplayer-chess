#include "ServerFunctions.hpp"
#include "ChessProtocolFunctions.hpp"

#include <iostream>

#define WAIT_MSG "wait\n"
#define CONNECT_MSG "connect\n"
#define DISCONNECT_MSG "disconnect\n"
#define MAX_SIZE 1024

void ServerFunctions::createNewConnection(sf::TcpListener& listener, sf::TcpSocket* client1, sf::TcpSocket* client2)
{
	if (listener.accept(*client1) != sf::Socket::Done)
		throw std::exception("Can not accept connection");

	std::cout << "Client1 connected \n";

	if (client1->send(WAIT_MSG, strlen(WAIT_MSG) + 1) != sf::Socket::Done)  //Send waiting msg to first client
		throw(std::exception("Client 1 is disconnected"));

	if (listener.accept(*client2) != sf::Socket::Done)
		throw(std::exception("Client 2 is disconnected"));

	std::cout << "Client2 connected \n";

	if (client1->send(CONNECT_MSG, strlen(CONNECT_MSG) + 1) != sf::Socket::Done)  //Send connect msg to first client
		throw(std::exception("Client 1 is disconnected"));
	
	if (client2->send(CONNECT_MSG, strlen(CONNECT_MSG) + 1) != sf::Socket::Done)  //Send connect msg to second client
		throw(std::exception("Client 2 is disconnected"));
}

int ServerFunctions::sendBoards(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	const char* white_gui_board = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
	const char* white_array_board = "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";
	const char* black_gui_board = "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr0";
	const char* black_array_board = "RNBKQBNRPPPPPPPP################################pppppppprnbKqbnr";

	if (sock1->send(white_gui_board, strlen(white_gui_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock2);
		return sf::Socket::Error;
	}
	if (sock1->send(white_array_board, strlen(white_array_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock2);
		return sf::Socket::Error;
	}
	if (sock2->send(black_gui_board, strlen(black_gui_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock1);
		return sf::Socket::Error;
	}
	if (sock2->send(black_array_board, strlen(black_array_board) + 1) != sf::Socket::Done)
	{
		ServerFunctions::sendDisconnectMsg(sock1);
		return sf::Socket::Error;
	}

	return sf::Socket::Done;
}

void ServerFunctions::sendDisconnectMsg(sf::TcpSocket* sock)
{
	sock->send(DISCONNECT_MSG, strlen(DISCONNECT_MSG) + 1);
}

void ServerFunctions::forward_msg(sf::TcpSocket* sock1, sf::TcpSocket* sock2) //, std::mutex sock1_mu, std::mutex sock2_mu)
{
	char data[MAX_SIZE];
	std::size_t received;
	while (true)
	{
		if (sock1->receive(data, MAX_SIZE, received) != sf::Socket::Done)
		{
			ServerFunctions::sendDisconnectMsg(sock2);
			break;
		}
		else
		{
		//	if (!ChessProtocol::isMsgCmd(data))
			//	ChessProtocol::convertData(data);
			
			std::string new_data(data);
			new_data = new_data.substr(0, new_data.find("\n")) + "\n";
			if (sock2->send(new_data.c_str(), new_data.length() + 1) != sf::Socket::Done)
			{
				ServerFunctions::sendDisconnectMsg(sock1);
				break;
			}
		}
	}
}

void ServerFunctions::matchThread(sf::TcpSocket* sock1, sf::TcpSocket* sock2)
{
	std::thread(ServerFunctions::forward_msg, sock1, sock2).detach();
	std::thread(ServerFunctions::forward_msg, sock2, sock1).detach();

}
