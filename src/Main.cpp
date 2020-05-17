#include <game/ClientGame.h>
#include <game/ServerGame.h>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

// ./game [client / server]
int main(int argc, char * argv[])
{
	// Clear logs
	std::remove("logs/game_log.txt");
	std::remove("logs/misc_log.txt");

	// Create loggers
	auto game_logger = spdlog::basic_logger_mt("game", "logs/game_log.txt");
	auto misc_logger = spdlog::basic_logger_mt("misc", "logs/misc_log.txt");
    spdlog::set_default_logger(game_logger);

	// Set loggers
	PrintUtil::_logger = spdlog::get("misc");

	if (argc != 2)
	{
		std::cerr << "usage: ./game [client / server]" << std::endl;
		return -1;
	}
	
	std::string option = argv[1];
	if (option == "server")
	{
		spdlog::info("Server is initializing");
		ServerGame game(9000);
	}
	else if (option == "client")
	{   
		spdlog::info("Client is initializing");
		std::string host ("localhost");
		ClientGame game(host, 9000);
	}
}