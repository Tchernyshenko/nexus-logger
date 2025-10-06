# IPC Logging System
A high-performance, cross-platform logging system built on Inter-Process Communication (IPC) for embedded systems and real-time applications. Originally designed for QNX Neutrino RTOS.

## 🚀 Features
- **High-Performance IPC**: Built on QNX Neutrino IPC mechanisms for minimal latency
- **Cross-Platform**: Supports QNX Neutrino and Linux systems
- **Thread-Safe**: Atomic operations and proper synchronization
- **RAII Resource Management**: Automatic cleanup and exception safety
- **Flexible Backends**: Extensible architecture for different logging outputs
- **Graceful Shutdown**: Proper signal handling and resource cleanup

## 🛠️ Components
### Core Classes
- BaseQnxComponent: IPC channel lifecycle management
- BaseQnxService: Message processing loop
- BaseLogger: Abstract logging interface
- LoggerService: Client-facing facade

## 📦 Quick Start

``` cpp
#include <nexus/logger/logger_macros.hpp>

int main() {
    // Initialize logging system
    auto logger = std::make_unique<nexus::logger::LoggerService>();
    nexus::logger::LoggerService::Initialize(std::move(logger));

    // Use logging macros
    LOG_INFO("Application started successfully");
    LOG_ERROR("Database connection failed");
    
    return 0;
}
```

## 🔧 Building
``` bash
mkdir build && cd build
cmake .. -DBUILD_QNX=ON
make
```

## 🎯 Use Cases
- Embedded Systems: Automotive, aerospace, medical devices
- Real-Time Applications: Financial trading, industrial automation
- Distributed Systems: Microservices communication tracing
- High-Reliability Systems: Where traditional logging may impact performance

## 🔒 Safety & Reliability
- Exception Safety: Strong guarantee on all public APIs
- Thread Safety: Atomic operations for cross-thread communication
- Signal Safety: Async-signal-safe signal handlers
- Resource Cleanup: RAII pattern for all system resources

## 📄 License
This project is licensed under the MIT License - see LICENSE file for details.

## 🤝 Contributing
- Fork the repository
- Create a feature branch
- Add tests for new functionality
- Submit a pull request

## 🐛 Bug Reports
Please use GitHub Issues to report bugs or request features.


