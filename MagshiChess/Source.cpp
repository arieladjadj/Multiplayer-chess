#include "Pipe.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>
#include <mutex>
#pragma warning(disable:4996)

std::mutex* mu;

using std::cout;
using std::endl;
using std::string;

void serverListener(sf::TcpSocket* sock, Pipe chatPipe, Pipe changePipe, Game* g);
void chatPipeListener(sf::TcpSocket* sock, Pipe chatPipe);
std::tuple<sf::TcpSocket*, Game*, string, string> connectToServer(Pipe& p, Pipe& change);
std::tuple<Pipe, Pipe, Pipe> connectToPipes();


int main()
{
	mu = new std::mutex;
	srand(time_t(NULL));
	string str4gui = "";  
	string str4game = "";
	sf::TcpSocket* sock = new sf::TcpSocket();
	Game* g = nullptr;
	Pipe p, change, chat;
	char msgToGraphics[10240];

	try {
		std::tie(p, change, chat) = connectToPipes();
		std::tie(sock, g, str4gui, str4game) = connectToServer(p, change);
	}
	catch (std::exception & e) {
		cout << e.what();
		_exit(0);
	}

	/* creating the listener threads and detaching them from the main thread */
	std::thread(serverListener, sock, chat, change, g).detach();
	std::thread(chatPipeListener, sock, chat).detach();

	strcpy_s(msgToGraphics, str4gui.c_str());
	p.sendMessageToGraphics(msgToGraphics);   // send the board string to graphics

	// get message from graphics
	string msgFromGraphics = p.getMessageFromGraphics();

	while (msgFromGraphics != "quit")
	{
		g->playTurn(msgFromGraphics);

		// get message from graphics
		msgFromGraphics = p.getMessageFromGraphics();
	}

	p.close();
	chat.close();
	change.close();
	delete sock;
	delete g;

	return 0;
}

void serverListener(sf::TcpSocket* sock, Pipe chatPipe, Pipe changePipe, Game* g)
{
	char data[10240] = { 0 };
	int srcRow = 0, srcCol = 0, dstRow = 0, dstCol = 0, guiDstRow = 0, guiSrcRow = 0;
	char crown = 0;
	string msg = "";
	string value = "";
	std::size_t received;
	
	while (true)
	{
		sock->receive(data, 10240, received);
		msg = string(data);
		if (msg.find("chat") != string::npos)
		{
			std::cout << "Got msg cmd";
			msg = "chat " + string(data);
			strcpy(data, msg.c_str());
			changePipe.sendMessageToGraphics(data);
			cout << "Received data " << endl;
			std::cout << "Sent to pipe: " << msg << endl;
		} // move 1,1 3,3
		else if (msg.find("move") != string::npos)
		{
			std::unique_lock<std::mutex> lock(*mu); // Locking the mutex and thus making sure there is no race condition
			if (msg.length() >= 14) { // If the opponent's move made a check
				strcpy(data, "msg 1"); // send to the pipe to display the check message
				changePipe.sendMessageToGraphics(data);
			}
			/* Converting the move positions to ints */
			srcRow = (msg[5] - '0');
			guiSrcRow = 7 - srcRow;
			srcCol = (msg[7] - '0');
			guiDstRow = 7 - (msg[9] - '0');
			dstRow = (msg[9] - '0');
			dstCol = (msg[11] - '0');
			g->getOtherPlayer()->makeMove(std::make_tuple(srcRow, srcCol, dstRow, dstCol));
			g->getOnlinePlayer()->makeMove(std::make_tuple(srcRow, srcCol, dstRow, dstCol));
			/* Creating the change messages */
			msg = "change ";
			msg += (char)(guiSrcRow + '0');
			msg += ',';
			msg += (char)(srcCol + '0');
			msg += ' ';
			msg += '#';
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			changePipe.sendMessageToGraphics(data); // sending the message to the graphics to display the opponent's move
			msg = "change ";
			msg += (char)(guiDstRow + '0');
			msg += ',';
			msg += (char)(dstCol + '0');
			msg += ' ';
			msg += g->getOtherPlayer()->getBoard()[dstRow][dstCol]->getPieceChar();
			strcpy(data, msg.c_str());
			data[msg.length() + 1] = 0;
			changePipe.sendMessageToGraphics(data);
			strcpy(data, "turn"); // changing the turn text in the graphics to the next player
			changePipe.sendMessageToGraphics(data);
			g->setCurrTurn(true);
		}
		else if (msg.find("mate") != string::npos)
		{
			if (g->getOnlinePlayer()->getColor() == Color::black) {
				strcpy(data, "mate black");
				changePipe.sendMessageToGraphics(data);
			}
			else if (g->getOnlinePlayer()->getColor() == Color::white) {
				strcpy(data, "mate white");
				changePipe.sendMessageToGraphics(data);
			}
		}
		else if (msg.find("crown") != string::npos)
		{
			std::unique_lock<std::mutex> lock(*mu); // locking the mutex to prevent Race Conditions from occuring
			/* Converting the positions to ints */
			crown = msg[10];
			srcRow = msg[6] - '0';
			srcCol = msg[8] - '0';
			guiSrcRow = 7 - srcRow;
			/* Replacing the current piece there to the chosen piece */
			delete g->getOnlinePlayer()->getBoard()[srcRow][srcCol];
			g->getOnlinePlayer()->getBoard()[srcRow][srcCol] = PipeInputOperations::getPieceFromChar(crown);
			delete g->getOtherPlayer()->getBoard()[srcRow][srcCol];
			g->getOtherPlayer()->getBoard()[srcRow][srcCol] = PipeInputOperations::getPieceFromChar(crown);
			/* Creating change messages */
			msg = "change ";
			msg += (char)guiSrcRow + '0';
			msg += ',';
			msg += (char)srcCol + '0';
			msg += ' ';
			msg += crown;
			strcpy(data, msg.c_str());
			changePipe.sendMessageToGraphics(data);
		}
		else  if (msg.find("disconnect") != string::npos)
		{
			strcpy(data, "disconnect");
			changePipe.sendMessageToGraphics(data); // send disconnect messsge upon client disconnection
			_exit(0);
		}
	}
}
void chatPipeListener(sf::TcpSocket* sock, Pipe chatPipe) {
	string msg = "";
	/* Listen to chat messages from the pipe */
	while (true) {
		msg = chatPipe.getMessageFromGraphics(); // get the chat message
		cout << "Received From Pipe: " << msg << endl;
		sock->send(msg.c_str(), msg.length() + 1); // forward it to the server
		cout << "Sent to server: " << msg << endl;
	}
}

std::tuple<sf::TcpSocket*, Game*, string, string> connectToServer(Pipe& p, Pipe& change){
	char msgToGraphics[10240];
	sf::TcpSocket* sock = new sf::TcpSocket();
	string str4gui = "";
	string str4game = "";
	Game* g;
	char data[10240];
	std::size_t receieved;
	if (sock->connect("45.32.177.133", 6000) != sf::Socket::Done) { // connecting to my server
		throw(std::exception("Connection to server failed"));
	}
	sock->receive(data, 10240, receieved); // getting the message if its wait, meaning that we are the first client
	if (string(data).find("wait") != std::string::npos){
		strcpy(msgToGraphics, "wait");
		change.sendMessageToGraphics(msgToGraphics); // send the message to the gui to signal that we are waiting for the client
		sock->receive(data, 10240, receieved);
		if (string(data).find("connect") != string::npos)
		{	//white
			strcpy(msgToGraphics, "connect");
			change.sendMessageToGraphics(msgToGraphics); // signal to the gui that a client connected
			/* make us the white player */
			str4gui = "rnbqkbnrpppppppp################################PPPPPPPPRNBQKBNR0";
			str4game = "rnbkqbnrpppppppp################################PPPPPPPPRNBKQBNR";

			g = new Game(str4game, p, change, mu, sock, Color::white); // create game object
			g->setCurrTurn(true);
		}
	}
	else if(string(data).find("connect") != string::npos) {  //black
		strcpy(msgToGraphics, "connect");
		change.sendMessageToGraphics(msgToGraphics); // signal to the gui that the player connected
		/* make us the black player */
		str4gui = "RNBKQBNRPPPPPPPP################################pppppppprnbkqbnr0";
		str4game = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";

		g = new Game(str4game, p, change, mu, sock, Color::black); // create game object
		g->setCurrTurn(false);
	}
	else {
		throw(std::exception("Connection to server failed"));
	}

	strcpy(msgToGraphics, str4gui.c_str());
	p.sendMessageToGraphics(msgToGraphics);   // send the board string

	return std::make_tuple(sock, g, str4gui, str4game);
}
std::tuple<Pipe, Pipe, Pipe> connectToPipes()
{
	bool isConnectP = false;
	bool isConnectChange = false;
	bool isConnectChat = false;
	Pipe p(0);
	Pipe change(1);
	Pipe chat(2);
	string ans;
	/* make sure the pipes are connected */
	while (!isConnectP and !isConnectChange and !isConnectChat)
	{
		try {
			isConnectP = p.connect();
			isConnectChange = change.connect();
			isConnectChat = chat.connect();
		}
		catch (...) {
		}

		if (isConnectP and isConnectChange and isConnectChat)
			break;

		cout << "cant connect to graphics" << endl;
		cout << "Do you try to connect again or exit? (0-try again, 1-exit)" << endl;
		std::cin >> ans;

		if (ans != "0")
		{
			try {
				p.close();
				change.close();
				chat.close();
			}
			catch (...){
				throw(std::exception("Pipes Error"));
			}
			throw(std::exception("Quiting..."));
		}
	}
	return std::make_tuple(p, change, chat);
}

