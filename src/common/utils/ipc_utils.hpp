#pragma once
#include <cstring>
#include <system_error>

// QNX
#include <sys/dispatch.h>

// Common
#include "../types/message_types.hpp"

namespace nexus::utils::ipc {

using namespace nexus::ipc;

// Безопасная отправка строкового сообщения
static bool SendMessage(const int connection_id, const MessageCode code,
                        const std::string& message) {
    if (connection_id == -1) {
        return false;
    }

    IpcMessage msg{};
    msg.code = code;

    // Копируем только нужную часть
    const size_t message_size
        = std::min(message.size(), sizeof(msg.text) - 1);
    std::memcpy(msg.text, message.c_str(), message_size);
    msg.text[message_size] = '\0';

    // Отправляем только заголовок + актуальные данные + 1 для null-terminator
    const size_t total_size = sizeof(MessageCode) + message_size + 1;
    return MsgSend(connection_id, &msg, total_size,
                   nullptr, 0) != -1;
}

// Отправка пульса
static bool SendPulse(const int connection_id, const int priority,
                      const int code, const int value = 0) {
    if (connection_id == -1) {
        return false;
    }

    return MsgSendPulse(connection_id, priority, code, value) != -1;
}

// Подключение к процессу с обработкой ошибок
static int ConnectToProcess(const std::string& channel_name) {
    const int coid = name_open(channel_name.c_str(), 0);
    if (coid == -1) {
        throw std::system_error(errno, std::system_category(),
                                "cannot connect to: " + channel_name);
    }
    return coid;
}

// Безопасное отключение
static void Disconnect(const int connection_id) {
    if (connection_id != -1) {
        ConnectDetach(connection_id);
    }
}

} // namespace nexus::common::ipc