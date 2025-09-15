#include "logging.h"
#include "logging/logging-core.h"
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

#define LOG_BUFFER_SIZE 4096 // 日志缓冲区大小，单个日志长度不能超过该值

static Logger *G_LOGGER = NULL; // 全局日志对象，唯一实例

/**
 * @brief 为日志添加一个handler
 * @param handler 处理器对象
 */
bool addHandler(Logger *logger, log_Handler *handler) {
    if (logger == NULL || handler == NULL) {
        return false;
    }
    if (logger->handler == NULL) {
        logger->handler = handler;
        return true;
    }

    logger->handler->_free(logger->handler);
    logger->handler = handler;
    return true;
}

/**
 * @brief 为日志添加一个filter
 * @param filter 过滤器对象
 */
bool addFilter(Logger *logger, log_filter *filter) {
    if (logger == NULL || filter == NULL) {
        return false;
    }
    if (logger->filter == NULL) {
        logger->filter       = filter;
        logger->filter->next = NULL;
        return true;
    }

    log_filter *it = logger->filter;
    while (it->next != NULL) {
        it = it->next;
    }

    it->next     = filter;
    filter->next = NULL;
    return true;
}

/**
 * @brief 输出到handler
 * @param handler 处理器对象
 * @param level 日志等级
 * @param color 应用的颜色
 * @param message 日志内容
 */
/**
 * @brief 输出到handler
 * @param handler 处理器对象
 * @param level 日志等级
 * @param color 应用的颜色
 * @param message 日志内容
 */
static void output_to_handler(Logger     *logger,
                              char       *level,
                              const char *color,
                              const char *message) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    char timeStr[20];
    getTimeStr(timeStr);
    char logStr[LOG_BUFFER_SIZE * 2];
    if (_logger->handler->apply_color) {
        snprintf(logStr,
                 LOG_BUFFER_SIZE * 2,
                 "[%s]: %s %s%s%s %s\n",
                 _logger->name,
                 timeStr,
                 color,
                 level,
                 RESET,
                 message);
    } else {
        snprintf(logStr,
                 LOG_BUFFER_SIZE * 2,
                 "[%s]: %s %s %s\n",
                 _logger->name,
                 timeStr,
                 level,
                 message);
    }

    _logger->handler->output(_logger->handler, logStr);
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
log_cope(Logger *logger, char *level, const char *color, const char *message) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger == NULL) {
        return;
    }
    if (_logger->handler == NULL) {
        return;
    }

    log_filter  *it      = _logger->filter;
    log_Handler *handler = _logger->handler;

    while (it != NULL) {
        if (it->_dispose(it, _logger->level, message)) {
            output_to_handler(_logger, level, color, message);
            if (it->jump_out) {
                return;
            }
        }
        it = it->next;
    }
    output_to_handler(_logger, level, color, message);
}

void _log_fatal(
    Logger *logger, const char *file, int line, const char *message, ...) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        char    finalLogStr[LOG_BUFFER_SIZE * 2];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);
        log_cope(_logger, "Fatal", RED_B, finalLogStr);
    }
}

void _log_error(
    Logger *logger, const char *file, int line, const char *message, ...) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger->level >= LOG_ERROR) {
        char    logStr[LOG_BUFFER_SIZE];
        char    finalLogStr[LOG_BUFFER_SIZE * 2];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);
        log_cope(_logger, "Error", RED, finalLogStr);
    }
}

void _log_warning(
    Logger *logger, const char *file, int line, const char *message, ...) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger->level >= LOG_WARNING) {
        char    logStr[LOG_BUFFER_SIZE];
        char    finalLogStr[LOG_BUFFER_SIZE * 2];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);
        log_cope(_logger, "Warning", YELLOW, finalLogStr);
    }
}

void _log_info(
    Logger *logger, const char *file, int line, const char *message, ...) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger->level >= LOG_INFO) {
        char    logStr[LOG_BUFFER_SIZE];
        char    finalLogStr[LOG_BUFFER_SIZE * 2];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);
        log_cope(_logger, "Info", GREEN, finalLogStr);
    }
}

void _log_debug(
    Logger *logger, const char *file, int line, const char *message, ...) {
    Logger *_logger = NULL;

    if (logger != NULL) {
        _logger = logger;
    } else {
        _logger = G_LOGGER;
    }

    if (_logger->level >= LOG_DEBUG) {
        char    logStr[LOG_BUFFER_SIZE];
        char    finalLogStr[LOG_BUFFER_SIZE * 2];
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);
        log_cope(_logger, "Debug", CYAN, finalLogStr);
    }
}

/**
 * @brief 初始化默认日志器
 * @param name 日志器名称
 * @param level 日志等级
 * @return 日志器对象
 */
void initDefaultLogger(const char *name, log_level level) {
    if (G_LOGGER != NULL) {
        G_LOGGER->name  = name;
        G_LOGGER->level = level;
    }

    Logger *logger  = (Logger *)malloc(sizeof(Logger));

    logger->level   = level;
    logger->handler = loggingHandlerConsole();
    logger->name    = name;
    logger->filter  = NULL;

    G_LOGGER        = logger;
}

Logger *getDefaultLogger(void) { return G_LOGGER; }

/**
 * @brief 获取日志器对象
 * @param name 日志器名称
 * @param level 日志等级
 * @return 日志器对象
 */
Logger *getLogger(const char *name, log_level level) {
    if (name == NULL) {
        return NULL;
    }

    Logger *logger  = (Logger *)malloc(sizeof(Logger));

    logger->level   = level;
    logger->handler = loggingHandlerConsole();
    logger->name    = name;
    logger->filter  = NULL;

    return logger;
}

/**
 * @brief 销毁日志对象
 */
void destroyDefaultLogger(void) {
    if (G_LOGGER != NULL) {
        if (G_LOGGER->handler != NULL) {
            G_LOGGER->handler->_free(G_LOGGER->handler);
        }

        if (G_LOGGER->filter != NULL) {
            log_filter *it   = G_LOGGER->filter;
            log_filter *next = NULL;
            while (it != NULL) {
                next = it->next;
                it->_free(it);
                it = next;
            }
        }

        free(G_LOGGER);
        G_LOGGER = NULL;
    }
}

void destroyLogger(Logger *logger) {
    if (logger != NULL) {
        if (logger->handler != NULL) {
            logger->handler->_free(logger->handler);
        }

        if (logger->filter != NULL) {
            log_filter *it   = logger->filter;
            log_filter *next = NULL;
            while (it != NULL) {
                next = it->next;
                it->_free(it);
                it = next;
            }
        }

        free(logger);
    }
}