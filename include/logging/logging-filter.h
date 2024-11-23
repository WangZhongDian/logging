#ifndef __LOGGING_INTERCEPTOR_H__
#define __LOGGING_INTERCEPTOR_H__

#include "logging-core.h"
#include "logging-handler.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct log_filter {
    log_level    level;
    log_Handler *handler;
    bool         jump_out;
    bool (*_dispose)(struct log_filter *filter,
                     log_level          level,
                     const char        *message,
                     ...);
    void (*_free)(struct log_filter *filter);
    struct log_filter *next;
} log_filter;

/**
 * @brief  子字符串过滤器
 * @param keywords: 关键字数组
 * @param count: 关键字数组长度
 * @param level: 过滤截日志等级
 * @param handler: 日志处理器,用于处理过滤下来的日志
 * @return log_filter *
 */
log_filter *loggingFilterSubStr(char        *keywords[],
                                log_level    level,
                                log_Handler *handler,
                                bool         jump_out);

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_INTERCEPTOR_H__