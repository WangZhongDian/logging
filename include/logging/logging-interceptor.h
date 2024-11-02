#ifndef __LOGGING_INTERCEPTOR_H__
#define __LOGGING_INTERCEPTOR_H__

#include "logging-core.h"
#include "logging-handler.h"

typedef struct log_Interceptor {
    log_level    level;
    log_Handler *handler;
    bool (*_dispose)(char *level, const char *message, ...);
    void (*_free)(struct log_Interceptor *Interceptor);

} log_Interceptor;

log_Interceptor *loggingSubStringInterceptor(char        *keywords[],
                                             int          count,
                                             log_level    level,
                                             log_Handler *handler);

#endif // __LOGGING_INTERCEPTOR_H__