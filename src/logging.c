/********************************************
 * @Date: 2024 08 12
 * @Description: 日志模块
 ********************************************/

#include "logging.h"
#include "logging/logging-handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Logger *G_LOGGER = NULL;

#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"
#define CYAN    "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define WHITE   "\033[0;37m"
#define BLACK   "\033[0;30m"

#define LOG_BUFFER_SIZE 1024

static void getTimeStr(char *timeStr) {
    time_t     t = time(NULL);
    struct tm *p = localtime(&t);
    char       _timeStr[20];
    strftime(_timeStr, sizeof(_timeStr), "%Y-%m-%d %H:%M:%S", p);
    strcpy(timeStr, _timeStr);
}

/**
 * @description : 添加日志处理器
 */
static void addHandler(log_Handler *handler) {
    if (G_LOGGER == NULL) {
        return;
    }
    if (G_LOGGER->handler == NULL) {
        G_LOGGER->handler = handler;
        return;
    }

    G_LOGGER->handler->_free(G_LOGGER->handler);

    G_LOGGER->handler = handler;
}

/**
 * @description : 添加日志拦截器
 * @param
 * @return
 */
void addInterceptor(log_Interceptor *Interceptor) {
    if (G_LOGGER == NULL) {
        return;
    }
    if (G_LOGGER->interceptor == NULL) {
        G_LOGGER->interceptor = Interceptor;
        return;
    }

    G_LOGGER->interceptor->_free(G_LOGGER->interceptor);

    G_LOGGER->interceptor = Interceptor;
}

/**
 * @description : 内置日志记录函数
 */
static void
_builtin_log(char *level, const char *color, const char *message, ...) {
    if (G_LOGGER == NULL) {
        return;
    }
    if (G_LOGGER->handler == NULL) {
        return;
    }
    char timeStr[20];
    getTimeStr(timeStr);
    char logStr[LOG_BUFFER_SIZE];

    log_Handler *handler = G_LOGGER->handler;

    // 通过拦截器
    if (G_LOGGER->interceptor != NULL) {
        if (G_LOGGER->interceptor->_dispose(level, message)) {
            if (G_LOGGER->interceptor->handler != NULL) {
                handler = G_LOGGER->interceptor->handler;
            }
        }
    }

    // 判断处理器是否应用颜色
    if (handler->apply_color)
        sprintf(
            logStr, "%s %s%s%s %s\n", timeStr, color, level, RESET, message);
    else
        sprintf(logStr, "%s %s %s\n", timeStr, level, message);

    handler->output(handler, logStr);
}

//*************************记录日志******************************* */
static void fatal(const char *message, ...) {
    if (G_LOGGER->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Fatal", RED, logStr, args);
    }
}

static void error(const char *message, ...) {
    if (G_LOGGER->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Error", RED, logStr, args);
    }
}

static void warning(const char *message, ...) {
    if (G_LOGGER->level >= LOG_WARNING) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Warning", YELLOW, logStr, args);
    }
}

static void info(const char *message, ...) {
    if (G_LOGGER->level >= LOG_INFO) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Info", GREEN, logStr, args);
    }
}

static void debug(const char *message, ...) {
    if (G_LOGGER->level >= LOG_DEBUG) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Debug", CYAN, logStr, args);
    }
}
//*************************记录日志******************************* */

/**
 * @description :获取一个日志操作对象
 * @param
 * @return
 */
static Logger *getLogger(const char *name, log_level level) {
    if (G_LOGGER != NULL) {
        return G_LOGGER;
    }
    
    Logger *logger         = (Logger *)malloc(sizeof(Logger));
    logger->fatal          = fatal;
    logger->error          = error;
    logger->warning        = warning;
    logger->info           = info;
    logger->debug          = debug;

    logger->addHandler     = addHandler;
    logger->addInterceptor = addInterceptor;

    logger->level          = level;
    logger->handler        = consoleHandler(name);
    logger->name           = name;
    logger->interceptor    = NULL;

    G_LOGGER               = logger;
    return G_LOGGER;
}

/**
 * @description :创建一个日志对象
 * @return :Logging* 返回一个日志对象
 */
Logging *createLogging() {
    Logging *logging   = (Logging *)malloc(sizeof(Logging));
    logging->getLogger = getLogger;
    return logging;
}

/**
 * @description :销毁日志对象
 */
log_status destroyLogging(Logging *logging) {
    if (logging == NULL) {
        return L_ERROR;
    }
    if (G_LOGGER != NULL) {
        if (G_LOGGER->handler != NULL) {
            G_LOGGER->handler->_free(G_LOGGER->handler);
        }

        if (G_LOGGER->interceptor != NULL) {
            G_LOGGER->interceptor->_free(G_LOGGER->interceptor);
        }

        free(G_LOGGER);
        G_LOGGER = NULL;
    }
    free(logging);
    return L_OK;
}

/**
 * @description :获取当前日志操作对象
 * @return 当前唯一的日志操作对象
 */
Logger *getCurrentLogger(void) {
    if (G_LOGGER == NULL) {
        return NULL;
    }
    return G_LOGGER;
}
