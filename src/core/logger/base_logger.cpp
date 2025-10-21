#include "base_logger.hpp"

namespace nexus::logger {

using namespace std::literals;

BaseLogger::BaseLogger(const std::string& name)
    : BaseQnxService(name) {
}

void BaseLogger::Run() {
    Write("Logger has been started."s);
    Flush();
    BaseQnxService::Run();
    Write("Logger has been stopped."s);
    Flush();
}

void BaseLogger::HandlePulse(const _pulse& ipc_pulse) {
    if (ipc_pulse.code == ipc::PULSE_SHUTDOWN) {
        Write("Received shutdown pulse - stopping..."s);
        Flush();
        Stop();
    }
}

void BaseLogger::HandleMessage([[maybe_unused]] int receive_id,
                               const ipc::IpcMessage& ipc_message) {

    using namespace utils;
    std::string message_time = time::ToString(time::GetCurrentTime());
    std::string message_header = GetMessageHeader(ipc_message.code);
    std::string message_text(ipc_message.text, sizeof(ipc_message.text));

    Write(message_time + message_header + message_text);
    Flush();
}

void BaseLogger::HandleReceiveError(const int error_code) {
    Write("Receive error: "s + std::string(strerror(error_code)));
    Flush();
}

std::string BaseLogger::GetMessageHeader(const ipc::MessageCode& code) {
    switch (code) {
        case ipc::LOG_INFO:
            return " [INFO] "s;
        case ipc::LOG_ERROR:
            return " [ERROR] "s;
        default:
            return " [UNKNOWN] "s;
    }
}

} // namespace nexus::common::logger