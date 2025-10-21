#include "base_qnx_service.hpp"

namespace nexus::ipc {
// Инициализация статических членов
std::atomic<bool> BaseQnxService::shutdown_requested_{false};

BaseQnxService::BaseQnxService(const std::string& server_name)
    : BaseQnxComponent(server_name), running_{false} {

    struct sigaction action = {};
    action.sa_handler = SignalHandler;
    sigemptyset(&action.sa_mask);

    if (sigaction(SIGINT, &action, nullptr) == -1) {
        throw std::system_error(errno, std::system_category(),
                                "Failed to set SIGINT handler");
    }

    if (sigaction(SIGTERM, &action, nullptr) == -1) {
        throw std::system_error(errno, std::system_category(),
                                "Failed to set SIGTERM handler");
    }
}

void BaseQnxService::Run() {
    if (running_.exchange(true)) {
        throw std::runtime_error("Service is already running");
    }

    while (running_.load(std::memory_order_acquire) &&
           !shutdown_requested_.load(std::memory_order_acquire)) {
        IpcBuffer buffer{};

        const int rcvid =
            MsgReceive(GetAttach()->chid, &buffer, sizeof(buffer), nullptr);

        if (rcvid == 0) {
            HandlePulse(buffer.ipc_pulse);
        } else if (rcvid > 0) {
            HandleMessage(rcvid, buffer.ipc_message);
            MsgReply(rcvid, 0, nullptr, 0);
        } else {
            // Если ошибка EINTR (прервано сигналом)
            if (errno == EINTR) {
                if (shutdown_requested_) {
                    Stop();
                }
            } else {
                HandleReceiveError(errno);
            }
        }
    }
}

void BaseQnxService::Stop() {
    if (!running_.exchange(false)) {
        return; // Уже остановлен
    }

    // Отправляем пульс, чтобы разблокировать MsgReceive
    if (GetAttach() && GetAttach()->chid != -1) {
        MsgSendPulse(GetAttach()->chid, getprio(0), _PULSE_CODE_UNBLOCK, 0);
    }
}

void BaseQnxService::SignalHandler(int signal) {
    shutdown_requested_.store(true, std::memory_order_release);
}
} // namespace nexus::common::ipc