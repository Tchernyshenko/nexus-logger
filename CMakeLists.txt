cmake_minimum_required(VERSION 3.15)
project(nexus_logger VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем выходную директорию для config_manager
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Все файлы вместе
set(SOURCES
        src/main.cpp

        # Base classes
        src/core/ipc/base_qnx_component.cpp
        src/core/ipc/base_qnx_component.hpp
        src/core/ipc/base_qnx_service.cpp
        src/core/ipc/base_qnx_service.hpp

        # Logger
        src/core/logger/base_logger.cpp
        src/core/logger/base_logger.hpp
        src/core/logger/logger_service.cpp
        src/core/logger/logger_service.hpp

        # Macros
        src/core/logger/logger_macros.hpp

        # Sinks
        src/sinks/console_logger.cpp
        src/sinks/console_logger.hpp
        src/sinks/file_logger.cpp
        src/sinks/file_logger.cpp

        # Utils & Types
        src/common/utils/time_utils.hpp
        src/common/utils/ipc_utils.hpp
        src/common/utils/path_utils.hpp

        src/common/types/message_types.hpp
        src/common/types/pulse_types.hpp
        src/common/types/channels_names.hpp
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