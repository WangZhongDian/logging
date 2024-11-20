#include "logging.h"
#include "logging/logging-handler.h"
#include "utils/logging-utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED    "\033[0;31m"
#define RED_B  "\033[0;41m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define RESET  "\033[0m"
#define CYAN   "\033[0;36m"

#define LOG_BUFFER_SIZE 1024

Logger *G_LOGGER = NULL;

/**
 * @brief 为日志添加一个handler
 * @param handler 处理器对象
 */
static bool addHandler(log_Handler *handler) {
    if (G_LOGGER == NULL || handler == NULL) {
        return false;
    }
    if (G_LOGGER->handler == NULL) {
        G_LOGGER->handler = handler;
        return true;
    }

    G_LOGGER->handler->_free(G_LOGGER->handler);
    G_LOGGER->handler = handler;
    return true;
}

static bool addInterceptor(log_Interceptor *Interceptor) {
    if (G_LOGGER == NULL || Interceptor == NULL) {
        return false;
    }
    if (G_LOGGER->interceptor == NULL) {
        G_LOGGER->interceptor = Interceptor;
        return true;
    }

    G_LOGGER->interceptor->_free(G_LOGGER->interceptor);
    G_LOGGER->interceptor = Interceptor;
    return true;
}

/**
 * @brief 内部日志打印处理核心函数
 * @param level 日志等级
 * @param color 应用的颜色
 * @param message 日志内容
 * @param ... 格式化参数列表
 * @return
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

    if (G_LOGGER->interceptor != NULL) {
        if (G_LOGGER->interceptor->_dispose(level, message)) {
            if (G_LOGGER->interceptor->handler != NULL) {
                handler = G_LOGGER->interceptor->handler;
            }
        }
    }

    if (handler->apply_color)
        sprintf(logStr,
                "[%s]: %s %s%s%s %s\n",
                G_LOGGER->name,
                timeStr,
                color,
                level,
                RESET,
                message);
    else
        sprintf(logStr,
                "[%s]: %s %s %s\n",
                G_LOGGER->name,
                timeStr,
                level,
                message);

    handler->output(handler, logStr);
}

static void fatal(const char *message, ...) {
    if (G_LOGGER->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_log("Fatal", RED_B, logStr, args);
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

Logger *newLogger(const char *name, log_level level) {
    if (G_LOGGER != NULL) {
        G_LOGGER->name  = name;
        G_LOGGER->level = level;
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
    logger->handler        = loggingConsoleHandler();
    logger->name           = name;
    logger->interceptor    = NULL;

    G_LOGGER               = logger;
    return G_LOGGER;
}

/**
 * @brief 销毁日志对象
 */
log_status destroyLogger(void) {
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
    return L_OK;
}

Logger *getDefaultLogger(void) {
    if (G_LOGGER == NULL) {
        return NULL;
    }
    return G_LOGGER;
}