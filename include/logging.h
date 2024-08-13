
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

typedef struct log_Handler {
    void* out;
    bool apply_color;
    void (*_free)(struct log_Handler* handler);//释放资源
} log_Handler;


typedef struct log_Interceptor {
    log_level level;                                        //拦截级别
    log_Handler* handler;                                   //拦截目标处理器
    bool (*_dispose)(char* level,const char *message, ...); //拦截触发器
    void (*_free)(struct log_Interceptor* Interceptor);     //释放资源
}  log_Interceptor;


//日志操作器
typedef struct Logger
{
    log_level level;
    log_Handler* handler;
    log_Interceptor* interceptor;
    const char* name;
    void (*fatal)(const char* format, ...);
    void (*error)(const char* format, ...);
    void (*warning)(const char* format, ...);
    void (*info)(const char* format, ...);
    void (*debug)(const char* format, ...);

    void (*addHandler)(log_Handler* handler);
    // void (*addFormat)(const char* format);
    // void (*addFilter)(const char* filter);
    void (*addInterceptor)(log_Interceptor* Interceptor);
} Logger;


//日志类对象
typedef struct Logging {
    Logger* (*getLogger)(const char* name, log_level level);
    log_status (*setLevel)(Logger* logger, log_level level);
} Logging;



Logging* createLogging();                        
log_status destroyLogging(Logging* logging);
Logger* getCurrentLogger(void);


/**
 * @def 创建日志处理器
 * @file logging-handler.c
*/
log_Handler* fileHandler(const char* name);     
log_Handler* consoleHandler(const char* name);



log_Interceptor* substringInterceptor(char *keywords[], int count, log_level level, log_Handler* handler);


#endif // __LOGGING_H