# Nexus Logger
Простой, но мощный асинхронный логгер для C++ с поддержкой QNX Neutrino. Предоставляет гибкую систему логирования с возможностью вывода в консоль, файл и другие приемники через IPC-механизмы.

## Архитектура
Проект использует многоуровневую архитектуру с четким разделением ответственности:
```
src/
├── common/                     # Общие утилиты и типы
│ ├── types/
│ │ ├── channels_names.hpp      # Имена IPC каналов
│ │ ├── message_types.hpp       # Типы IPC сообщений
│ │ └── pulse_types.hpp         # Типы системных пульсов
│ └── utils/
│ ├── ipc_utils.hpp             # Утилиты для работы с IPC
│ ├── path_utils.hpp            # Утилиты для работы с путями
│ └── time_utils.hpp            # Утилиты для работы со временем
├── core/                       # Ядро системы
│ ├── ipc/
│ │ ├── base_qnx_component.hpp  # Базовый QNX IPC компонент
│ │ ├── base_qnx_component.cpp
│ │ ├── base_qnx_service.hpp    # Базовый QNX IPC сервис
│ │ └── base_qnx_service.cpp
│ └── logger/
│ ├── base_logger.hpp           # Базовый абстрактный логгер
│ ├── base_logger.cpp
│ ├── logger_service.hpp        # Фасад для клиентского использования
│ ├── logger_service.cpp
│ └── logger_macros.hpp         # Макросы для удобного логирования
├── sinks/                      # Реализации приемников логирования
│ ├── console_logger.hpp        # Вывод в консоль
│ ├── console_logger.cpp
│ ├── file_logger.hpp           # Вывод в файл
│ └── file_logger.cpp
└── main.cpp                    # Демонстрационное приложение
```

##  Ключевые компоненты
### Базовые классы (core/)
**BaseQnxComponent** - RAII-обертка для QNX IPC, обеспечивающая:
- Валидацию имен серверов
- Автоматическое управление ресурсами (name_attach/name_detach)
- Безопасную очистку с отправкой пульсов разблокировки

**BaseQnxService** - базовый сервис для обработки IPC сообщений:
- Главный цикл обработки сообщений (MsgReceive/MsgReply)
- Обработка сигналов graceful shutdown (SIGINT/SIGTERM)
- Потокобезопасное управление состоянием

**BaseLogger** - абстрактный логгер с поддержкой IPC:
- Обработка структурированных лог-сообщений
- Форматирование с временными метками и уровнями
- Шаблонный метод для различных бэкендов

### Приемники логирования (sinks/)

**ConsoleLogger** - вывод в стандартный поток:
```cpp
nexus::logger::ConsoleLogger logger("logger");
logger.Run();
```
**FileLogger** - вывод в файл с созданием директорий:
```cpp
nexus::logger::FileLogger logger("logger", "/var/log/nexus.log");
logger.Run();
```
### Клиентский интерфейс (core/logger/)

**LoggerService** - фасад для клиентского использования:
- Singleton с потокобезопасной инициализацией
- Автоматическое переподключение при разрыве соединения
- Простой API для отправки сообщений

**Logger Macros** - макросы для удобного использования:
```cpp
LOG_INFO("Сообщение");
LOG_ERROR("Ошибка");
```

### Общие утилиты (common/)
**Типы данных**:
- message_types.hpp - структуры IPC сообщений
- pulse_types.hpp - коды системных пульсов
- channels_names.hpp - имена IPC каналов

**Утилиты**:
- ipc_utils.hpp - функции для работы с IPC
- time_utils.hpp - работа со временем
- path_utils.hpp - работа с файловыми путями
- 
## Использование

``` cpp
#include "sinks/console_logger.hpp"
#include "core/logger/logger_service.hpp"

int main() {
    // Создание и запуск логгера
    nexus::logger::ConsoleLogger logger(nexus::channels::logger);
    std::thread logger_thread([&logger]() {
        logger.Run();
    });

    // Инициализация клиентского сервиса
    auto service = std::make_unique<nexus::logger::LoggerService>();
    nexus::logger::LoggerService::Initialize(std::move(service));

    // Использование
    LOG_INFO("Работаем...");

    // Graceful shutdown
    logger.Stop();
    logger_thread.join();
}
```

## Сборка
### Сборка из-под CLion с тулчейном KPDA для QNX Neutrino x86

1. **Настройка тулчейна** в CLion:
    - Откройте `File` → `Settings` → `Build, Execution, Deployment` → `Toolchains`
    - Добавьте новый тулчейн с именем `KPDA`
    - Укажите пути к компиляторам:
        - **C Compiler**: `/opt/kpda2021e/host/linux64/x86_64/usr/bin/ntox86-gcc-8.3.0`
        - **C++ Compiler**: `/opt/kpda2021e/host/linux64/x86_64/usr/bin/ntox86-g++-8.3.0`
        - **Debugger**: `/opt/kpda2021e/host/linux64/x86_64/usr/bin/ntox86-gdb-7.6.1`

2. **Настройка CMake профиля**:
    - Перейдите в `Settings` → `Build, Execution, Deployment` → `CMake`
    - Создайте новый профиль (например, `QNX-Release`)
    - Выберите тулчейн `KPDA`
    - В поле `CMake options` добавьте:
      ```
      -DCMAKE_BUILD_TYPE=Release
      ```

3. **Сборка проекта**:
    - Выберите созданный профиль в панели сборки
    - Нажмите `Build Project` или `Ctrl+F9`

### Сборка из командной строки

```bash
# Создаем директорию для сборки
mkdir build-qnx && cd build-qnx

# Конфигурируем проект с QNX компиляторами
cmake .. \
  -DCMAKE_C_COMPILER=/opt/kpda2021e/host/linux64/x86_64/usr/bin/ntox86-gcc-8.3.0 \
  -DCMAKE_CXX_COMPILER=/opt/kpda2021e/host/linux64/x86_64/usr/bin/ntox86-g++-8.3.0 \
  -DCMAKE_BUILD_TYPE=Release

# Собираем проект
make
```

## Запуск на целевой системе QNX
**Безопасность**:
- Потокобезопасные операции с атомарными флагами
- Исключительная безопасность в конструкторах/деструкторах
- Границы проверки для всех строковых операций

**Производительность**:
- Асинхронная обработка сообщений через IPC
- Минимальное копирование данных
- Эффективное управление памятью

**Расширяемость**:
- Легко добавить новые приемники (сеть, БД, etc.)
- Поддержка пользовательских типов сообщений
- Гибкая система уровней логирования

## Лицензия
Этот проект распространяется под лицензией MIT. Подробнее см. в файле LICENSE.


