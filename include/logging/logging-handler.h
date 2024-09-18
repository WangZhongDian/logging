#ifndef __LOGGING_HANDLER_H__
#define __LOGGING_HANDLER_H__

#include <stdbool.h>

typedef struct log_Handler {
    void* stream;
    bool apply_color;
    void (*_free)(struct log_Handler* handler);//释放资源
    void (*output)(struct log_Handler* handler,const char* message);
} log_Handler;


log_Handler* fileHandler(const char* name);     
log_Handler* consoleHandler(const char* name);

#endif  //__LOGGING_HANDLER_H__