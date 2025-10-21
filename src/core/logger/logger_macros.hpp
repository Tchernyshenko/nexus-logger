#pragma once

/**
 * @file logger_macros.hpp
 * @brief Макросы для удобного использования системы логирования
 */

#include "logger_service.hpp"

/**
 * @def LOG_INFO(message)
 * @brief Макрос для отправки информационного сообщения
 * @param message Сообщение для логирования (может быть строкой или выражением)
 */
#define LOG_INFO(message) \
    do { \
        if (auto* logger = ::nexus::logger::LoggerService::Instance()) { \
            logger->SendInfo(std::string(message)); \
        } \
    } while(0)

/**
 * @def LOG_ERROR(message)
 * @brief Макрос для отправки сообщения об ошибке
 * @param message Сообщение для логирования (может быть строкой или выражением)
 */
#define LOG_ERROR(message) \
    do { \
        if (auto* logger = ::nexus::logger::LoggerService::Instance()) { \
            logger->SendError(std::string(message)); \
        } \
    } while(0)
