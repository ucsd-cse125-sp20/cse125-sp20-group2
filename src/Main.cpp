#include <game/ClientGame.h>
#include <game/ServerGame.h>
#include <string>
#include <util/Config.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <Windows.h>

// Config loader variables
// Declared here because static variables need to be declared outside of class
std::unordered_map<std::string, std::string>* Config::client_vars;
std::unordered_map<std::string, std::string>* Config::server_vars;
std::unordered_map<std::string, std::string>* Config::updated_client_vars;
std::unordered_map<std::string, std::string>* Config::updated_server_vars;

// ./game [client / server]
int main(int argc, char * argv[])
{
	srand(time(NULL));
	
	// Load config data
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
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		ServerGame game(9000);
	}
	else if (option == "client")
	{
		ClientGame game("localhost", 9000);
	}

	// Unload config data
	Config::unload();
}


