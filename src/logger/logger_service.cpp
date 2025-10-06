#include "logger_service.hpp"

namespace nexus::logger {

std::unique_ptr<LoggerService> LoggerService::instance_ = nullptr;

LoggerService::~LoggerService() {
    utils::ipc::Disconnect(logger_coid_);
}

void LoggerService::Initialize(std::unique_ptr<LoggerService> logger) {
    instance_ = std::move(logger);

    // Подключаемся к логгер-процессу
    if (instance_) {
        try {
            instance_->logger_coid_ = utils::ipc::ConnectToProcess(
                channels::LOGGER);
        } catch (const std::exception& e) {
            // Логгер может быть недоступен - это нормально
            instance_->logger_coid_ = -1;
        }
    }
}

LoggerService* LoggerService::Instance() noexcept {
    return instance_.get();
}

void LoggerService::SendInfo(const std::string& message) const {
    if (logger_coid_ != -1) {
        utils::ipc::SendMessage(logger_coid_, ipc::LOG_INFO,
                                this->name_ + ' ' + message);
    }
}

void LoggerService::SendError(const std::string& message) const {
    if (logger_coid_ != -1) {
        utils::ipc::SendMessage(logger_coid_, ipc::LOG_ERROR,
                                this->name_ + ' ' + message);
    }
}

void LoggerService::SetLogName(const std::string& name) {
    name_ = name;
}

}