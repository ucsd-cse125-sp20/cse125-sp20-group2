#include <util/PrintUtil.h>

std::shared_ptr<spdlog::logger> PrintUtil::_logger = NULL;

void PrintUtil::print(Game::ServerMessage msg) {
    // if (PrintUtil::_logger == NULL) {
    //     std::cout << "This is called 1" << std::endl;
    //     PrintUtil::_logger = spdlog::get("misc");
    // }
    PrintUtil::_logger->info(msg.DebugString());
}

void PrintUtil::print(Game::ClientMessage msg) {
    // if (PrintUtil::_logger == NULL) {
    //     std::cout << "This is called 2" << std::endl;
    //     PrintUtil::_logger = spdlog::get("misc");
    // }
    PrintUtil::_logger->info(msg.DebugString());
}