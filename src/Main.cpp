
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include "./network/NetworkServer.h"
#include "./network/NetworkClient.h"

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
		
		while (1) {
			std::unordered_map<unsigned int, std::vector<std::string>> stuff = server.readAllMessages();
			std::unordered_map<unsigned int, std::vector<std::string>>::iterator iter;
			for (iter = stuff.begin(); iter != stuff.end(); iter++) {
				for (std::string data : iter->second) {
					std::cout << iter->first << ". msg: " << data << std::endl;
					server.send(iter->first, std::string ("i gotchu:") + data);
				}
				
			}
			std::cout << "sleep" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	else if (option == "client")
	{   
		NetworkClient client("localhost", 9000);
		int i = 0;
		while (i < 100) {
			std::string s ("msg:");
			s += std::to_string(i++);
			client.send(s);
			std::cout << "Sent message: " << s << std::endl;
			std::string rec = client.read();
			std::cout << "Recv message: " << rec << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


