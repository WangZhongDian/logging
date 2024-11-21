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
        G_LOGGER->interceptor       = Interceptor;
        G_LOGGER->interceptor->next = NULL;
        return true;
    }

    log_Interceptor *it = G_LOGGER->interceptor;
    while (it->next != NULL) {
        it = it->next;
    }

    it->next          = Interceptor;
    Interceptor->next = NULL;
    return true;
}

static void output_to_handler(log_Handler *handler,
                              char        *level,
                              const char  *color,
                              const char  *message) {
    char timeStr[20];
    getTimeStr(timeStr);
    char logStr[LOG_BUFFER_SIZE];
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

/**
 * @brief 内部日志打印处理核心函数
 * @param level 日志等级
 * @param color 应用的颜色
 * @param message 日志内容
 * @param ... 格式化参数列表
 * @return
 */
static void _builtin_cope(char *level, const char *color, const char *message) {
    if (G_LOGGER == NULL) {
        return;
    }
    if (G_LOGGER->handler == NULL) {
        return;
    }

    log_Interceptor *it      = G_LOGGER->interceptor;
    log_Handler     *handler = G_LOGGER->handler;

    while (it != NULL) {
        if (it->_dispose(it, level, message)) {
            output_to_handler(it->handler, level, color, message);
            if (it->jump_out)
                return;
        }
        it = it->next;
    }
    output_to_handler(handler, level, color, message);
}

void log_fatal(const char *message, ...) {
    if (G_LOGGER->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_cope("Fatal", RED_B, logStr);
    }
}

void log_error(const char *message, ...) {
    if (G_LOGGER->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_cope("Error", RED, logStr);
    }
}

void log_warning(const char *message, ...) {
    if (G_LOGGER->level >= LOG_WARNING) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_cope("Warning", YELLOW, logStr);
    }
}

void log_info(const char *message, ...) {
    if (G_LOGGER->level >= LOG_INFO) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_cope("Info", GREEN, logStr);
    }
}

void log_debug(const char *message, ...) {
    if (G_LOGGER->level >= LOG_DEBUG) {
        char    logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        _builtin_cope("Debug", CYAN, logStr);
    }
}

Logger *newDefaultLogger(const char *name, log_level level) {
    if (G_LOGGER != NULL) {
        G_LOGGER->name  = name;
        G_LOGGER->level = level;
        return G_LOGGER;
    }

    Logger *logger         = (Logger *)malloc(sizeof(Logger));

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
log_status destroyDefaultLogger(void) {
    if (G_LOGGER != NULL) {
        if (G_LOGGER->handler != NULL) {
            G_LOGGER->handler->_free(G_LOGGER->handler);
        }

        if (G_LOGGER->interceptor != NULL) {
            log_Interceptor *it   = G_LOGGER->interceptor;
            log_Interceptor *next = NULL;
            while (it != NULL) {
                next = it->next;
                it->_free(it);
                it = next;
            }
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