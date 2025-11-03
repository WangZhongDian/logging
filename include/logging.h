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
    loggingMessage(NULL, LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_error(format, ...)                                                 \
    loggingMessage(NULL, LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_warning(format, ...)                                               \
    loggingMessage(NULL, LOG_WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_info(format, ...)                                                  \
    loggingMessage(NULL, LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Log_debug(format, ...)                                                 \
    loggingMessage(NULL, LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

// 日志器宏
#define log_fatal(logger, format, ...)                                         \
    loggingMessage(logger, LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_error(logger, format, ...)                                         \
    loggingMessage(logger, LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_warning(logger, format, ...)                                       \
    loggingMessage(                                                            \
        logger, LOG_WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_info(logger, format, ...)                                          \
    loggingMessage(logger, LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define log_debug(logger, format, ...)                                         \
    loggingMessage(logger, LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

typedef struct Logger {
    log_level    level;
    log_Handler *handler;
    log_filter  *filter;
    const char  *name;
} Logger;

bool loggingAddHandler(Logger *logger, log_Handler *handler);
bool loggingAddFilter(Logger *logger, log_filter *filter);

void loggingMessage(Logger     *logger,
                    log_level   level,
                    const char *file,
                    int         line,
                    const char *message,
                    ...);

/**
 * @brief 创建一个日志句柄对象
 * @param name 日志器名称
 * @return 日志句柄对象
 */
Logger *loggingNewLogger(const char *name);
/**
 * @brief 获取默认日志对象
 * @return 默认日志对象
 */
Logger *loggingGetDefaultLogger(void);

Logger *loggingGetLogger(const char *name);

Logger *getLogger(const char *name);

/**
 * @brief 销毁日志对象,该方法会销毁默认日志对象
 */
void loggingDestroyAll(void);

/**
 * @brief 销毁日志对象
 * @param logger 日志对象
 * @return void
 */
void loggingDestroyLogger(Logger *logger);

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_H__