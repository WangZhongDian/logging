#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

static void __freeConsoleHandler(log_Handler *handler) { free(handler); }

static void outputConsoleHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

log_Handler *loggingConsoleHandler() {
    log_Handler *handler = (log_Handler *)malloc(sizeof(log_Handler));

    handler->stream      = stdout;
    handler->apply_color = true;
    handler->_free       = __freeConsoleHandler;
    handler->output      = outputConsoleHandler;

    return handler;
}