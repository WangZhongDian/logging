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

/**
 * @brief 文件处理器
 * @param name 文件名
 * @param max_size 文件最大大小
 * @return
 */
log_Handler *loggingHandlerFile(const char *name, unsigned int max_size);

/**
 * @brief 控制台处理器
 * @param
 * @return
 */
log_Handler *loggingHandlerConsole();

#ifdef __cplusplus
}
#endif

#endif //__LOGGING_HANDLER_H__