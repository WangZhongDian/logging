#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#include "logging/logging-core.h"
#include "logging/logging-filter.h"
#include "logging/logging-handler.h"

#ifdef __cplusplus
extern "C" {
#endif

// 默认日志器宏
#define Log_fatal(format, ...)                                                 \
    _log_fatal(NULL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_error(format, ...)                                                 \
    _log_error(NULL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_warning(format, ...)                                               \
    _log_warning(NULL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_info(format, ...)                                                  \
    _log_info(NULL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_debug(format, ...)                                                 \
    _log_debug(NULL, __FILE__, __LINE__, format, ##__VA_ARGS__)

// 日志器宏
#define log_fatal(logger, format, ...)                                         \
    _log_fatal(logger, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_error(logger, format, ...)                                         \
    _log_error(logger, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_warning(logger, format, ...)                                       \
    _log_warning(logger, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_info(logger, format, ...)                                          \
    _log_info(logger, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_debug(logger, format, ...)                                         \
    _log_debug(logger, __FILE__, __LINE__, format, ##__VA_ARGS__)

typedef struct Logger {
    log_level    level;
    log_Handler *handler;
    log_filter  *filter;
    const char  *name;
} Logger;

bool addHandler(Logger *logger, log_Handler *handler);
bool addFilter(Logger *logger, log_filter *filter);

void _log_fatal(
    Logger *logger, const char *file, int line, const char *format, ...);
void _log_error(
    Logger *logger, const char *file, int line, const char *format, ...);
void _log_warning(
    Logger *logger, const char *file, int line, const char *format, ...);
void _log_info(
    Logger *logger, const char *file, int line, const char *format, ...);
void _log_debug(
    Logger *logger, const char *file, int line, const char *format, ...);

/**
 * @brief 初始化默认日志对象
 * @param name 日志名称
 * @param level 日志等级
 */
void initDefaultLogger(const char *name, log_level level);

/**
 * @brief 获取默认日志对象
 * @return 默认日志对象
 */
Logger *getDefaultLogger(void);

Logger *getLogger(const char *name, log_level level);

/**
 * @brief 销毁日志对象,该方法会销毁默认日志对象
 */
void destroyDefaultLogger(void);

/**
 * @brief 销毁日志对象
 * @param logger 日志对象
 * @return void
 */
void destroyLogger(Logger *logger);

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_H__