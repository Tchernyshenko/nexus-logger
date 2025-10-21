#pragma once
#include <fstream>

// Base
#include "../core/logger/base_logger.hpp"

namespace nexus::logger {
class FileLogger final : public BaseLogger {
public:
    explicit FileLogger(const std::string& server_name, std::string filepath);
    ~FileLogger() override;

protected:
    void Write(std::string formatted_message) override;
    void Flush() override;

private:
    std::ofstream file_;
    std::string filepath_;
};
} // namespace nexus::logger