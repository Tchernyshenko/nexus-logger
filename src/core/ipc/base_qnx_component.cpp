#include "base_qnx_component.hpp"

namespace {
constexpr size_t MAX_SERVER_NAME_LENGTH = 64;

bool IsValidServerName(const std::string& server_name) {
    //  Проверка длины
    if (server_name.empty() || server_name.length() > MAX_SERVER_NAME_LENGTH) {
        return false;
    }

    //  Проверка символов с помощью алгоритма
    return std::all_of(server_name.begin(), server_name.end(),
                       [](const char c) {
                           return std::isalnum(static_cast<unsigned char>(c)) ||
                                  c == '_' || c == '-';
                       });
}
} // namespace

namespace nexus::ipc {
BaseQnxComponent::BaseQnxComponent(const std::string& server_name) {
    // Валидация имени сервера
    if (!IsValidServerName(server_name)) {
        throw std::invalid_argument("Invalid server name");
    }

    // Регистрация имени в пространстве имен
    attach_ = name_attach(nullptr, server_name.c_str(), 0);
    if (attach_ == nullptr) {
        throw std::system_error(errno, std::system_category());
    }
}


BaseQnxComponent::~BaseQnxComponent() {
    if (!attach_) {
        return;
    }

    try {
        // Отправка пульса для разблокировки ожидающих потоков
        if (attach_->chid != -1) {
            MsgSendPulse(attach_->chid, getprio(0), _PULSE_CODE_UNBLOCK, 0);
        }

        // Отсоединение имени от пространства имен
        name_detach(attach_, 0);
    } catch (...) {
        // Гарантия отсутствия исключений в деструкторе
    }

    attach_ = nullptr;
}
} // namespace nexus::common::ipc