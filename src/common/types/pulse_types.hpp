#pragma once

#include <sys/neutrino.h>

namespace nexus::ipc {

#pragma pack(push, 1)
enum PulseCode : uint8_t {
    PULSE_SHUTDOWN = _PULSE_CODE_MAXAVAIL
};
#pragma pack(pop)

}