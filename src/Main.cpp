#include <game/ClientGame.h>
#include <game/ServerGame.h>
#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

// ./game [client / server]
int main(int argc, char * argv[])
{
	auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/game_log.txt");
    spdlog::set_default_logger(file_logger);

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


