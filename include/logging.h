
#ifndef __LOGGING_H
#define __LOGGING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef enum {
    LOG_FATAL,
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
    void* out;
    bool apply_color;
} log_Handler;


//日志操作器
typedef struct Logger
{
    log_level level;
    log_Handler* handler;
    const char* name;
    void (*fatal)(const char* format, ...);
    void (*error)(const char* format, ...);
    void (*warning)(const char* format, ...);
    void (*info)(const char* format, ...);
    void (*debug)(const char* format, ...);

    void (*addHandler)(log_Handler* handler);
    // void (*addFormat)(const char* format);
    // void (*addFilter)(const char* filter);
} Logger;


//日志类对象
typedef struct Logging {
    Logger* (*getLogger)(const char* name, log_level level);
    log_status (*setLevel)(Logger* logger, log_level level);
} Logging;



Logging* createLogging();                        //创建日志操作器
log_status destroyLogging(Logging* logging);
Logger* getCurrentLogger(void);


log_Handler* fileHandler(const char* name);     //文件处理器
log_Handler* consoleHandler(const char* name); //控制台处理器


#endif // __LOGGING_H