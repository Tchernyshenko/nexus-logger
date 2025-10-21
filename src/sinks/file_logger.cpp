#include "file_logger.hpp"
#include <stdexcept>
#include <utility>

// Utils
#include "common/utils/path_utils.hpp"

namespace nexus::logger {
FileLogger::FileLogger(const std::string& server_name,
                       std::string filepath)
    : BaseLogger(server_name), filepath_(std::move(filepath)) {
    // Создаем директорию если нужно
    if (!utils::path::EnsureDirectoryExists(filepath_)) {
        throw std::runtime_error("Cannot create directory for log file: "
                                 + filepath_);
    }

    file_.open(filepath_, std::ios::app);
    if (!file_.is_open()) {
        throw std::runtime_error("Cannot open log file: " + filepath_);
    }
}

FileLogger::~FileLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void FileLogger::Write(const std::string formatted_message) {
    if (file_.is_open()) {
        file_ << formatted_message << '\n';
    }
}

void FileLogger::Flush() {
    if (file_.is_open()) {
        file_.flush();
    }
}
} // namespace nexus::logger