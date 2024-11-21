#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdarg.h>
#include <stdbool.h>

#include "logging/logging-core.h"
#include "logging/logging-handler.h"
#include "logging/logging-interceptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Logger {
    log_level        level;
    log_Handler     *handler;
    log_Interceptor *interceptor;
    const char      *name;
    bool (*addHandler)(log_Handler *handler);
    bool (*addInterceptor)(log_Interceptor *Interceptor);
} Logger;

void log_fatal(const char *format, ...);
void log_error(const char *format, ...);
void log_warning(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);

/**
* @brief
创建默认日志对象,日志对象为单例模式，后续可通过getDefaultLogger方法获取，
        重复调用该方法不会创建新的日志对象，只会返回默认日志对象，并且会修改默认日志对象的名称和等级
* @param name 日志名称
* @param level 日志等级
* @return Logger* 日志对象指针
*/
Logger *newDefaultLogger(const char *name, log_level level);

/**
 * @brief 获取默认日志对象
 */
Logger *getDefaultLogger(void);

/**
 * @brief 销毁日志对象,该方法会销毁默认日志对象
 */
log_status destroyDefaultLogger(void);

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_H__