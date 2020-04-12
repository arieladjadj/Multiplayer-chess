#include "ServerFunctions.hpp"

int main() 
{
	sf::TcpListener listener;
	std::vector<std::thread*> threads;
	std::thread* match;
	bool createdListeneSock = true;

	if (listener.listen(5000) != sf::Socket::Done)
		createdListeneSock = false;

	while (createdListeneSock)
	{
		sf::TcpSocket* client1 = new sf::TcpSocket;
		sf::TcpSocket* client2 = new sf::TcpSocket;

		try {
			ServerFunctions::createNewConnection(listener, client1, client2);
		}
		catch (std::exception & e)
		{
			continue;
		}

		match = new std::thread(ServerFunctions::matchThread, client1, client2);
		match->detach();
		threads.push_back(match);
	}

	for (auto thread : threads) 
	{
		delete thread;
	}
	return 0;
}

