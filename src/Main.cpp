#include <game/ClientGame.h>
#include <game/ServerGame.h>
#include <string>
#include <util/Config.h>

// Config loader variables
// Declared here because static variables need to be declared outside of class
std::unordered_map<std::string, std::string>* Config::vars;

// ./game [client / server]
int main(int argc, char * argv[])
{
	// Load config data
	Config::vars = new std::unordered_map<std::string, std::string>();
	Config::load();

	// Invalid usage - wrong # of args
	if (argc < 2)
	{
		std::cerr << "usage: ./game [client / server]" << std::endl;
		return -1;
	}

	std::string host ("localhost");
	if (argc > 2) {
		host = argv[2];
	}
	
	std::string option = argv[1];
	if (option == "server")
	{
		ServerGame game(9000);
	}
	else if (option == "client")
	{
		ClientGame game(host, 9000);
	}

	// Unload config data
	Config::unload();
}


