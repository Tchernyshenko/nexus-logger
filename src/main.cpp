#include <iostream>
#include <thread>
#include <chrono>

#include "implementation/console_logger.hpp"
#include "logger/logger_service.hpp"
#include "logger/logger_macros.hpp"
#include "types/channels_names.hpp"

// Простое демо-приложение, которое использует логгер
class DemoApplication {
public:
    void Run() {
        LOG_INFO("Demo application started");

        // Имитация работы приложения
        for (int i = 0; i < 5; ++i) {
            LOG_INFO("Processing item " + std::to_string(i));
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (i == 2) {
                LOG_ERROR("Simulated error occurred!");
            }
        }

        LOG_INFO("Demo application finished");
    }
};

int main() {
    try {
        std::cout << "=== Nexus Logger Demo ===\n";

        // Инициализация логгера
        nexus::logger::ConsoleLogger logger(nexus::channels::LOGGER);

        // Запускаем логгер в отдельном потоке
        std::thread logger_thread([&logger]() {
            logger.Run();
        });

        // Даем время логгеру запуститься и зарегистрировать IPC канал
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Инициализация сервиса логирования для клиентов
        auto logger_service = std::make_unique<nexus::logger::LoggerService>();
        nexus::logger::LoggerService::Initialize(std::move(logger_service));

        std::cout << "Logger started. Starting demo...\n\n";

        // Демонстрация 1: Базовое использование
        DemoApplication app;
        app.Run();

        std::cout << "\n--- Performance demo ---\n";

        // Демонстрация 2: Производительность
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 10; ++i) {
            // Уменьшил до 10 для скорости
            LOG_INFO("Performance test message " + std::to_string(i));
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start);

        LOG_INFO(
            "Logged 10 messages in " + std::to_string(duration.count()) + "ms");

        // Останавливаем логгер - отправляем пульс извне
        std::cout << "Stopping logger...\n";

        // Подключаемся к логгеру и отправляем пульс shutdown
        int coid = name_open(nexus::channels::LOGGER.c_str(), 0);
        if (coid != -1) {
            // Отправляем пульс shutdown
            MsgSendPulse(coid, 20, nexus::ipc::PULSE_SHUTDOWN, 0);
            name_close(coid);
        }
        // Ждем завершения логгера
        if (logger_thread.joinable()) {
            logger_thread.join();
        }

        std::cout << "\n=== Demo completed ===\n";
        return EXIT_SUCCESS;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}