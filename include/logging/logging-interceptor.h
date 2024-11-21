#ifndef __LOGGING_INTERCEPTOR_H__
#define __LOGGING_INTERCEPTOR_H__

#include "logging-core.h"
#include "logging-handler.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct log_Interceptor {
    log_level    level;
    log_Handler *handler;
    bool         jump_out;
    bool (*_dispose)(struct log_Interceptor *Interceptor,
                     char                   *level,
                     const char             *message,
                     ...);
    void (*_free)(struct log_Interceptor *Interceptor);
    struct log_Interceptor *next;
} log_Interceptor;

/**
 * @brief  子字符串拦截器
 * @param keywords: 关键字数组
 * @param count: 关键字数组长度
 * @param level: 最低拦截日志等级
 * @param handler: 日志处理器,用于处理拦截下来的日志
 * @return log_Interceptor *
 */
log_Interceptor *loggingSubStringInterceptor(char        *keywords[],
                                             log_level    level,
                                             log_Handler *handler,
                                             bool         jump_out);

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_INTERCEPTOR_H__