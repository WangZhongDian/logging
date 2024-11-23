#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 根据log_Handler结构体指针起始获取log_Handler_file_ex_t结构体指针
// log_Handler_file_ex_t与log_Handler处于连续内存中
// 使用char*指针进行偏移，达到以偏移1个字节为单位的偏移
#define Handler_file_EX_PRT(handler)                                           \
    ((log_Handler_file_ex_t *)((char *)handler + sizeof(log_Handler)))

#define FILE_NAME_MAX_SIZE 50

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
    free(Handler_file_EX_PRT(handler)->file_name);
    free(handler);
}

static void changeFile(log_Handler *handler) {
    log_Handler_file_ex_t *handler_ex = Handler_file_EX_PRT(handler);
    fclose(handler->stream);
    char new_file_name[FILE_NAME_MAX_SIZE];
    sprintf(new_file_name,
            "%s_%d.log",
            handler_ex->file_name,
            ++handler_ex->suffix);
    handler->stream       = fopen(new_file_name, "at");
    handler_ex->file_size = getFileSize(handler->stream);
}

static void outputFileHandler(log_Handler *handler, const char *message) {
    fputs(message, handler->stream);
    log_Handler_file_ex_t *handler_ex = Handler_file_EX_PRT(handler);
    handler_ex->file_size += strlen(message);
    if (handler_ex->file_size > handler_ex->file_size_max)
        changeFile(handler);
}

log_Handler *loggingHandlerFile(const char *name, unsigned int max_size) {
    char                   new_file_name[FILE_NAME_MAX_SIZE];
    int                    suffix = 0;
    unsigned int           file_size;
    FILE                  *fp         = NULL;
    log_Handler           *handler    = NULL;
    log_Handler_file_ex_t *handler_ex = NULL;

    do {
        sprintf(new_file_name, "%s_%d.log", name, suffix++);
        fp = fopen(new_file_name, "at");
        if (fp == NULL)
            goto ERROR;
        file_size = getFileSize(fp);
    } while (file_size > max_size);

    /// 分配log_Handler与记录文件大小的空间
    handler = (log_Handler *)malloc(sizeof(log_Handler) +
                                    sizeof(log_Handler_file_ex_t));
    if (handler == NULL)
        goto ERROR;

    handler_ex                = Handler_file_EX_PRT(handler);
    handler_ex->file_size_max = max_size;
    handler_ex->file_size     = file_size;
    handler_ex->suffix        = suffix;
    handler_ex->file_name     = strdup(name);
    if (handler_ex->file_name == NULL)
        goto ERROR;

    handler->stream      = fp;
    handler->apply_color = false;
    handler->_free       = __freeFileHandler;
    handler->output      = outputFileHandler;
    return handler;

ERROR:
    if (fp)
        fclose(fp);
    if (handler) {
        free(Handler_file_EX_PRT(handler)->file_name); // 直接释放，无需检查NULL
        free(handler);
    }
    return NULL;
}