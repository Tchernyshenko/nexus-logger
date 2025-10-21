#include "console_logger.hpp"
#include <iostream>

namespace nexus::logger {
ConsoleLogger::ConsoleLogger(const std::string& name)
    : BaseLogger(name) {
}

void ConsoleLogger::Write(std::string formatted_message) {
    std::cout << formatted_message << '\n';
}

void ConsoleLogger::Flush() {
    std::cout.flush();
}
} // namespace nexus::logger