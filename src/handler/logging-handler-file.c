/********************************************
 * @Date: 2024 09 18
 * @Description: 文件日志处理器
 ********************************************/
#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @description ：文件日志处理器释放组件
 * @param
 * @return
 */
static void __freeFileHandler(log_Handler *handler) {
    fclose(handler->stream);
    free(handler);
}

/**
 * @description :文件日志处理器输出组件
 * @param
 * @return
 */
static void outputFileHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

/**
 * @description ：文件日志处理器
 * @param
 * @return
 */
log_Handler *fileHandler(const char *name) {
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