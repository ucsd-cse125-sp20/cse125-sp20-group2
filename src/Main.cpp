
#include <iostream>
#include <chrono>
#include <thread>
#include "NetworkServer.h"
#include "NetworkClient.h"


// ./game [client / server]
int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: ./game [client / server]" << std::endl;
		return -1;
	}
	
	std::string option = argv[1];
	if (option == "server")
	{
		ServerNetwork server (9000);
	}
	else if (option == "client")
	{   
		NetworkClient client("localhost", 9000);
		while (1) {
			std::cout << "Sent message" << std::endl;
			char* buf = "bye";
			NetworkService::sendMessage(client.ConnectSocket, buf, DEFAULT_BUFLEN);
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
}


