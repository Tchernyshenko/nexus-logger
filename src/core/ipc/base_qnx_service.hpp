#pragma once

/**
 * @file base_qnx_service.hpp
 * @brief Базовый сервис для обработки IPC сообщений и пульсов в QNX
 */

#include <atomic>

// Base
#include "base_qnx_component.hpp"

// Utils
#include "../../common/utils/ipc_utils.hpp"

namespace nexus::ipc {

/**
 * @class BaseQnxService
 * @brief Базовый класс для сервисов, обрабатывающих IPC сообщения и пульсы
 *
 * Предоставляет основной цикл обработки сообщений, управление жизненным циклом
 * сервиса и обработку сигналов для graceful shutdown.
 * Наследники должны реализовать обработку конкретных типов сообщений и пульсов.
 */
class BaseQnxService : public BaseQnxComponent {
public:
    /**
     * @brief Конструктор базового QNX сервиса
     * @param server_name Имя сервера для регистрации в IPC
     * @throw std::invalid_argument При невалидном имени сервера
     * @throw std::system_error При ошибках создания IPC-канала
     */
    explicit BaseQnxService(const std::string& server_name);

    /**
     * @brief Виртуальный деструктор для корректного удаления наследников
     */
    ~BaseQnxService() override = default;

    /**
     * @brief Запуск основного цикла обработки сообщений
     *
     * Метод блокирует выполнение до вызова Stop() или получения сигнала завершения.
     * В цикле обрабатываются входящие сообщения и пульсы.
     *
     * @throw std::system_error При ошибках в IPC механизмах
     */
    void Run();

    /**
     * @brief Остановка сервиса и выход из цикла обработки
     *
     * Атомарно устанавливает флаг остановки и отправляет пульс для разблокировки
     * потока, ожидающего сообщения в MsgReceive().
     *
     * @note Потокобезопасность: thread-safe, может вызываться из любого потока
     */
    void Stop();

private:
    /**
     * @brief Обработка входящих пульсов
     * @param ipc_pulse Ссылка на структуру пульса
     *
     * Виртуальный метод для обработки системных и пользовательских пульсов.
     * Наследники должны реализовать специфичную логику обработки.
     */
    virtual void HandlePulse(const _pulse& ipc_pulse) = 0;

    /**
     * @brief Обработка входящих IPC сообщений
     * @param ipc_message Ссылка на IPC сообщение
     *
     * Виртуальный метод для обработки структурированных сообщений.
     * Наследники должны реализовать маршрутизацию и обработку разных типов сообщений.
     */
    virtual void HandleMessage(int receive_id, const IpcMessage& ipc_message) = 0;

    /**
     * @brief Обработка ошибок приема сообщений
     * @param error Код ошибки из errno
     *
     * Виртуальный метод для обработки ошибок, возникающих при MsgReceive().
     * Наследники могут реализовать логику восстановления или специфичное логирование.
     */
    virtual void HandleReceiveError(int error) = 0;

    /// @brief Атомарный флаг состояния работы сервиса
    std::atomic<bool> running_;

    /// @brief Статический атомарный флаг запроса завершения для всех экземпляров
    static std::atomic<bool> shutdown_requested_;

    /**
     * @brief Обработчик сигналов ОС для graceful shutdown
     * @param signal Номер полученного сигнала
     *
     * Статический метод, регистрируется как обработчик для SIGINT, SIGTERM.
     * Устанавливает флаг shutdown_requested_ и запускает процедуру остановки.
     *
     * @note Сигнало-безопасный: использует только атомарные операции и async-signal-safe функции
     */
    static void SignalHandler(int signal);
};

} // namespace nexus::ipc