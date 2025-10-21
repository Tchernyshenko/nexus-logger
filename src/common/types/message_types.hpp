#pragma once

//QNX
#include <sys/neutrino.h>

namespace nexus::ipc {

enum MessageCode : uint8_t {
    LOG_INFO = 0x30,
    LOG_ERROR = 0x31
};

#pragma pack(push, 1)
struct IpcMessage {
    MessageCode code;
    char text[5119];
};
#pragma pack(pop)

union IpcBuffer {
    _pulse ipc_pulse;
    IpcMessage ipc_message;
};

}