#pragma once

/**
 * @file logger_service.hpp
 * @brief Фасад для клиентского использования системы логирования
 */

#include <memory>
#include <string>

// Utils
#include "../internal/ipc_utils.hpp"

// Types
#include "../types/channels_names.hpp"

namespace nexus::logger {

class LoggerService {
public:
    virtual ~LoggerService();

    /**
     * @brief Инициализация глобального экземпляра логгера
     * @param logger Уникальный указатель на сервис логирования
     */
    static void Initialize(std::unique_ptr<LoggerService> logger);

    /**
     * @brief Получить глобальный экземпляр логгера
     * @return Указатель на сервис логирования или nullptr если не инициализирован
     */
    static LoggerService* Instance() noexcept;

    /**
     * @brief Отправить информационное сообщение
     * @param message Текст сообщения
     */
    virtual void SendInfo(const std::string& message) const;

    /**
     * @brief Отправить сообщение об ошибке
     * @param message Текст сообщения
     */
    virtual void SendError(const std::string& message) const;

    /**
     * @brief Установить имя логгера
     * @param name Имя для идентификации в логах
     */
    void SetLogName(const std::string& name);

private:
    static std::unique_ptr<LoggerService> instance_;

    mutable int logger_coid_{-1};
    std::string name_;
};

} // namespace nexus::logger