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
	}
	else if (option == "client")
	{   
		std::string host ("localhost");
		ClientGame game(host, 9000);
	}
}


