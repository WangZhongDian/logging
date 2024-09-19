#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>
#include <stdbool.h>

#include "logging/logging-core.h"
#include "logging/logging-handler.h"
#include "logging/logging-interceptor.h"

// 日志操作器
typedef struct Logger {
    log_level        level;
    log_Handler     *handler;
    log_Interceptor *interceptor;
    const char      *name;
    void (*fatal)(const char *format, ...);
    void (*error)(const char *format, ...);
    void (*warning)(const char *format, ...);
    void (*info)(const char *format, ...);
    void (*debug)(const char *format, ...);

    void (*addHandler)(log_Handler *handler);
    void (*addInterceptor)(log_Interceptor *Interceptor);
} Logger;

// 日志类对象
typedef struct Logging {
    Logger *(*getLogger)(const char *name, log_level level);
    log_status (*setLevel)(Logger *logger, log_level level);
} Logging;

Logging   *createLogging();                  // 创建日志类对象
log_status destroyLogging(Logging *logging); // 销毁日志类对象
Logger    *getCurrentLogger(void);           // 获取当前日志记录器

#endif // __LOGGING_H__