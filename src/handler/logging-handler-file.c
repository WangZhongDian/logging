#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Handler_FILE_EX_PRT(fp)                                                \
    ((log_Handler_file_ex_t *)(fp + sizeof(log_Handler)))

typedef struct log_Handler_file_ex_s {
    unsigned int file_size;
    unsigned int file_size_max;
    unsigned int suffix;
    char        *file_name;
} log_Handler_file_ex_t;

static unsigned int getFileSize(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    return ftell(fp);
}

static void __freeFileHandler(log_Handler *handler) {
    fclose(handler->stream);
    free(Handler_FILE_EX_PRT(handler)->file_name);
    free(handler);
}

static void changeFile(log_Handler *handler) {
    log_Handler_file_ex_t *handler_ex = Handler_FILE_EX_PRT(handler);
    fclose(handler->stream);
    char new_file_name[30];
    sprintf(new_file_name, "%s_%d.log", handler_ex->file_name, ++handler_ex->suffix);
    handler->stream = fopen(new_file_name, "at");
    handler_ex->file_size = getFileSize(handler->stream);
}

static void outputFileHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
    log_Handler_file_ex_t *handler_ex = Handler_FILE_EX_PRT(handler);
    handler_ex->file_size += strlen(message);
    if (handler_ex->file_size > handler_ex->file_size_max)
        changeFile(handler);
}

log_Handler *loggingFileHandler(const char *name, unsigned int max_size) {
    char         new_file_name[30];
    int          suffix = 0;
    unsigned int file_size;
    FILE        *fp;

    do {
        sprintf(new_file_name, "%s_%d.log", name, suffix++);
        fp        = fopen(new_file_name, "at");
        file_size = getFileSize(fp);
    } while (file_size > max_size);

    /// 分配log_Handler与记录文件大小的空间
    log_Handler           *handler = (log_Handler *)malloc(sizeof(log_Handler) +
                                                 sizeof(log_Handler_file_ex_t));

    log_Handler_file_ex_t *handler_ex = Handler_FILE_EX_PRT(handler);
    handler_ex->file_size_max        = max_size;
    handler_ex->file_size             = file_size;
    handler_ex->suffix                = suffix;
    handler_ex->file_name             = malloc(strlen(new_file_name) + 1);
    strcpy(handler_ex->file_name, name);

    handler->stream                   = fp;
    handler->apply_color              = false;
    handler->_free                    = __freeFileHandler;
    handler->output                   = outputFileHandler;

    return handler;
}