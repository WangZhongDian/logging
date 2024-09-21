#ifndef __LOGGING_INTERCEPTOR_H__
#define __LOGGING_INTERCEPTOR_H__

#include "logging-core.h"
#include "logging-handler.h"

typedef struct log_Interceptor {
    log_level    level;   // 拦截级别
    log_Handler *handler; // 拦截目标处理器
    bool (*_dispose)(char *level, const char *message, ...); // 拦截触发器
    void (*_free)(struct log_Interceptor *Interceptor);      // 释放资源

} log_Interceptor;

log_Interceptor *loggingSubStringInterceptor(char        *keywords[],
                                             int          count,
                                             log_level    level,
                                             log_Handler *handler);

#endif // __LOGGING_INTERCEPTOR_H__