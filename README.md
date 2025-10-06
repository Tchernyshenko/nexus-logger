# Nexus Logger - High-Performance IPC Logging System
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

## 📦 Quick Demo

### **Live Demonstration**
The project includes a complete working demo that showcases the logging system in action:

```bash
git clone https://github.com/Tchernyshenko/nexus-logger
cd nexus-logger
mkdir build && cd build
cmake .. -DBUILD_QNX=ON
make
```

## 🚀 Run the interactive demo
```
./bin/nexus_logger
```
### Expected Demo Output
```
=== Nexus Logger Demo ===
Logger started. Starting demo...

[2024-01-15 10:30:25] [INFO] Demo application started
[2024-01-15 10:30:25] [INFO] Processing item 0
[2024-01-15 10:30:25] [INFO] Processing item 1
[2024-01-15 10:30:26] [INFO] Processing item 2
[2024-01-15 10:30:26] [ERROR] Simulated error occurred!
[2024-01-15 10:30:26] [INFO] Processing item 3
[2024-01-15 10:30:26] [INFO] Processing item 4
[2024-01-15 10:30:26] [INFO] Demo application finished

--- Performance demo ---
[2024-01-15 10:30:26] [INFO] Performance test message 0
...
[2024-01-15 10:30:26] [INFO] Logged 100 messages in 45ms

Stopping logger...
[2024-01-15 10:30:27] [INFO] Logger has been stopped.

=== Demo completed ===
```

## 🔧 Building
```
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


