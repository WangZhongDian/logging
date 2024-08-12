
#ifndef __LOGGING_H
#define __LOGGING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

typedef enum {
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} log_level;

typedef enum {
    L_ERROR,
    L_OK,
} log_status;

typedef struct Handler {
    FILE* file;
    log_level level;
} log_Handler;


//日志操作器
typedef struct Logger
{
    log_level level;
    log_Handler* handler;
    void (*error)(const char* format, ...);
    void (*warning)(const char* format, ...);
    void (*info)(const char* format, ...);
    void (*debug)(const char* format, ...);
} Logger;


//日志类对象
typedef struct Logging {
    Logger* (*getLogger)(const char* name, log_level level);
    log_status (*setLevel)(Logger* logger, log_level level);
    // void (*addFormat)(const char* format);
    // void (*addFilter)(const char* filter);
    void (*addHandler)(log_Handler* handler);
} Logging;



Logging* createLogging();


log_Handler* fileHandler(const char* name,log_level level);

#endif // __LOGGING_H