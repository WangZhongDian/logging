/********************************************
 * @Date: 2024 09 18
 * @Description: 文件日志处理器
 ********************************************/
#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief 文件日志处理器释放组件
 */
static void __freeFileHandler(log_Handler *handler) {
    fclose(handler->stream);
    free(handler);
}

/**
 * @brief 文件日志处理器输出组件
 * @param handler 文件日志处理器
 * @param message 消息
 */
static void outputFileHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

/**
 * @brief 文件日志处理器
 * @param   name 文件名
 * @return  log_Handler *
 */
log_Handler *loggingFileHandler(const char *name) {
    char new_file_name[100];
    sprintf(new_file_name, "%s.log", name);

    FILE *fp             = fopen(new_file_name, "at");

    log_Handler *handler = (log_Handler *)malloc(sizeof(log_Handler));

    handler->stream      = fp;
    handler->apply_color = false;
    handler->_free       = __freeFileHandler;
    handler->output      = outputFileHandler;

    return handler;
}