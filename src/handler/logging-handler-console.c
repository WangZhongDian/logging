#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

static void __freeHandlerConsole(log_Handler *handler) { free(handler); }

static void __outputHandlerConsole(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

log_Handler *loggingHandlerConsole() {
    log_Handler *handler = (log_Handler *)malloc(sizeof(log_Handler));

    handler->stream      = stdout;
    handler->apply_color = true;
    handler->_free       = __freeHandlerConsole;
    handler->output      = __outputHandlerConsole;

    return handler;
}