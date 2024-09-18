/********************************************
 * @Date: 2024 09 18
 * @Description: 控制台日志处理器
 ********************************************/
#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @description 释放组件
 * @param
 */
static void __freeConsoleHandler(log_Handler *handler) { free(handler); }

/**
 * @description 输出组件
 * @param
 */
static void outputConsoleHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

/**
 * @description ：控制台日志处理器
 * @param
 */
log_Handler *consoleHandler(const char *name) {
    log_Handler *handler = (log_Handler *)malloc(sizeof(log_Handler));

    handler->stream      = stdout;
    handler->apply_color = true;
    handler->_free       = __freeConsoleHandler;
    handler->output      = outputConsoleHandler;

    return handler;
}