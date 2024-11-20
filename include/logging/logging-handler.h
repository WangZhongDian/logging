#ifndef __LOGGING_HANDLER_H__
#define __LOGGING_HANDLER_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct log_Handler {
    void *stream;
    bool  apply_color;
    void (*_free)(struct log_Handler *handler);
    void (*output)(struct log_Handler *handler, const char *message);
} log_Handler;

log_Handler *loggingFileHandler(const char *name, unsigned int max_size);
log_Handler *loggingConsoleHandler();

#ifdef __cplusplus
}
#endif

#endif //__LOGGING_HANDLER_H__