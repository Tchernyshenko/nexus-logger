#pragma once

/**
 * @file base_qnx_component.hpp
 * @brief Базовый компонент для QNX IPC систем
 */

#include <string>
#include <algorithm>
#include <stdexcept>
#include <system_error>

// QNX headers
#include <sys/dispatch.h>

namespace nexus::ipc {

/**
 * @class BaseQnxComponent
 * @brief Базовый класс для компонентов, использующих QNX IPC
 *
 * Обеспечивает управление жизненным циклом IPC-каналов и валидацию имен серверов.
 * Использует RAII для безопасного управления ресурсами.
 */

class BaseQnxComponent {
public:
    /**
     * @brief Конструктор с валидацией имени сервера
     * @param server_name Имя сервера для регистрации в пространстве имен
     * @throw std::invalid_argument При невалидном имени сервера
     * @throw std::system_error При ошибках создания IPC-канала
     */
    explicit BaseQnxComponent(const std::string& server_name);

    virtual ~BaseQnxComponent();

    // Запрещаем копирование и перемещение
    BaseQnxComponent(const BaseQnxComponent&) = delete;
    BaseQnxComponent& operator=(const BaseQnxComponent&) = delete;
    BaseQnxComponent(BaseQnxComponent&&) = delete;
    BaseQnxComponent& operator=(BaseQnxComponent&&) = delete;

    /**
     * @brief Получить указатель на структуру присоединенного имени
     * @return Константный указатель на name_attach_t
     */
    const name_attach_t* GetAttach() const noexcept {
        return attach_;
    }

protected:
    name_attach_t* attach_{nullptr};
};

} // namespace nexus::ipc