cmake_minimum_required(VERSION 3.15)
project(nexus_logger VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем выходную директорию для config_manager
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Все файлы вместе
set(SOURCES
        src/main.cpp

        # Base QNX
        src/ipc/base_qnx_component.cpp
        src/ipc/base_qnx_component.hpp
        src/ipc/base_qnx_service.cpp
        src/ipc/base_qnx_service.hpp

        # Logger
        src/logger/base_logger.cpp
        src/logger/base_logger.hpp
        src/logger/logger_service.cpp
        src/logger/logger_service.hpp

        # Implementations
        src/implementation/console_logger.cpp
        src/implementation/console_logger.hpp
        src/implementation/file_logger.cpp
        src/implementation/file_logger.cpp

        # Utils & Types
        src/internal/time_utils.hpp
        src/internal/ipc_utils.hpp
        src/internal/path_utils.hpp
        src/types/message_types.hpp
        src/types/pulse_types.hpp
        src/types/channels_names.hpp

        # Macros
        src/logger/logger_macros.hpp
)

# Просто собираем библиотеку
add_executable(nexus_logger ${SOURCES})

# Стандарт C++
set_target_properties(nexus_logger PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
)

# Include директории
target_include_directories(nexus_logger
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)