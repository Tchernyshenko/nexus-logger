#include "logger_service.hpp"

#include <iostream>

// Common
#include "common/types/channels_names.hpp"

namespace nexus::logger {
std::unique_ptr<LoggerService> LoggerService::instance_ = nullptr;
std::mutex LoggerService::mutex_;

LoggerService::~LoggerService() {
    utils::ipc::Disconnect(logger_coid_);
}

void LoggerService::Initialize(std::unique_ptr<LoggerService> logger) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (instance_) {
        return;
    }

    instance_ = std::move(logger);

    if (instance_) {
        try {
            instance_->logger_coid_ = utils::ipc::ConnectToProcess(channels::LOGGER);
        } catch (const std::exception& e) {
            instance_->logger_coid_ = -1;
            std::cerr << "Failed to connect to logger: " << e.what() << std::endl;
        }
    }
}

LoggerService* LoggerService::Instance() noexcept {
    return instance_.get();
}

bool LoggerService::IsConnected() const {
    return logger_coid_ != -1;
}

void LoggerService::Reconnect() const {
    if (logger_coid_ != -1) {
        utils::ipc::Disconnect(logger_coid_);
    }

    try {
        logger_coid_ = utils::ipc::ConnectToProcess(channels::LOGGER);
    } catch (const std::exception& e) {
        logger_coid_ = -1;
    }
}

void LoggerService::SendInfo(const std::string& message) {
    if (!IsConnected()) {
        Reconnect();
    }

    utils::ipc::SendMessage(logger_coid_, ipc::LOG_INFO,
                            this->name_ + ' ' + message);
}

void LoggerService::SendError(const std::string& message) {
    if (!IsConnected()) {
        Reconnect();
    }

    utils::ipc::SendMessage(logger_coid_, ipc::LOG_ERROR,
                            this->name_ + ' ' + message);
}

void LoggerService::SetLogName(const std::string& name) {
    name_ = name;
}
}