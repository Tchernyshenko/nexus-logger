#pragma once

/**
 * @file logger_service.hpp
 * @brief Фасад для клиентского использования системы логирования
 */

#include <memory>
#include <mutex>
#include <string>

// Utils
#include "../../common/utils/ipc_utils.hpp"

namespace nexus::logger {
class LoggerService final {
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
    * @brief Проверка соединения с логгером
    * @return Результат проверки соединения
    */
    bool IsConnected() const;

    /**
    * @brief Переподключиться к каналу логгера
    */
    void Reconnect() const;

    /**
     * @brief Отправить информационное сообщение
     * @param message Текст сообщения
     */
    virtual void SendInfo(const std::string& message);

    /**
     * @brief Отправить сообщение об ошибке
     * @param message Текст сообщения
     */
    virtual void SendError(const std::string& message);

    /**
     * @brief Установить имя логгера
     * @param name Имя для идентификации в логах
     */
    void SetLogName(const std::string& name);

private:
    static std::unique_ptr<LoggerService> instance_;
    static std::mutex mutex_;

    mutable int logger_coid_{-1};
    std::string name_;
};
} // namespace nexus::logger