#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>

static void __freeFileHandler(log_Handler *handler) {
    fclose(handler->stream);
    free(handler);
}

static void outputFileHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
}

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