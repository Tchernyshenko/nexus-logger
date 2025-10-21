#pragma once

// Base
#include "../core/logger/base_logger.hpp"

namespace nexus::logger {
class ConsoleLogger final : public BaseLogger {
public:
    explicit ConsoleLogger(const std::string& name);

protected:
    void Write(std::string formatted_message) override;
    void Flush() override;
};
} // namespace nexus::loger