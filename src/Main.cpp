#include <game/ClientGame.h>
#include <game/ServerGame.h>
#include <string>

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
		ServerGame game(9000);
		// ServerNetwork server (9000);
			
		// while (1) {
		// 	std::unordered_map<unsigned int, std::vector<Game::ClientMessage>> stuff = server.readAllMessages();
		// 	for (auto iter = stuff.begin(); iter != stuff.end(); iter++) {
		// 		Game::ServerMessage serverMessage;
		// 		serverMessage.set_net(Game::Net::PING);
		// 		server.send(iter->first, serverMessage);
				
		// 		for (auto clientMessage: iter->second) {
		//             switch (clientMessage.event_case()) {
		//                 case Game::ClientMessage::EventCase::kDirection:
		//                     std::cout << "Direction: " << clientMessage.direction() << std::endl;
		//                 break;
		//                 case Game::ClientMessage::EventCase::kNet:
		//                     std::cout << "Net: " << clientMessage.net() << std::endl;
		//                 break;
		//                 case Game::ClientMessage::EVENT_NOT_SET:
		//                     std::cout << "undefined" << std::endl;
		//                 break;
		//             }
		// 		}
		// 	}
		// 	std::cout << "sleep" << std::endl;

		// 	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		// }
	}
	else if (option == "client")
	{   
		std::string host ("localhost");
		ClientGame game(host, 9000);
		// NetworkClient client (host, 9000);

		// while (true) {
		// 	// Construct clientMessage
		// 	for (int i = 0; i < 4; i++) {
		// 		Game::ClientMessage clientMessage;

		// 		// Sets the clientMessage to send up msg
		// 		// clientMessage.set_direction(Game::Direction::DOWN);
		// 		clientMessage.set_direction(static_cast<Game::Direction>(i));

		// 		// Client sends a clientMessage now
		// 		client.send(clientMessage);
		// 	}

		// 	client.read();

		// 	for (auto serverMessage: client.messages) {
		// 		switch (serverMessage.event_case()) {
		// 			case Game::ServerMessage::EventCase::kNet:
		// 				std::cout << "Net " << serverMessage.net() << std::endl;
		// 				break;
		// 		}
		// 	}

		// 	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		// }
	}
}


