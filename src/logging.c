#include "logging.h"
#include "logging/logging-core.h"
#include "logging/logging-handler.h"
#include "utils/logging-map.h"
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

static Logger *ROOT_LOGGER = NULL; // 根日志对象，唯一实例

static Map *LOGGER_MAP     = NULL; // 日志对象映射表

/**
 * @brief 为日志添加一个handler
 * @param handler 处理器对象
 */
bool loggingAddHandler(Logger *logger, log_Handler *handler) {
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
bool loggingAddFilter(Logger *logger, log_filter *filter) {
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

    char timeStr[20];
    getTimeStr(timeStr);
    char logStr[LOG_BUFFER_SIZE * 2];
    if (logger->handler->apply_color) {
        snprintf(logStr,
                 LOG_BUFFER_SIZE * 2,
                 "[%s]: %s %s%s%s %s\n",
                 logger->name,
                 timeStr,
                 color,
                 level,
                 RESET,
                 message);
    } else {
        snprintf(logStr,
                 LOG_BUFFER_SIZE * 2,
                 "[%s]: %s %s %s\n",
                 logger->name,
                 timeStr,
                 level,
                 message);
    }

    logger->handler->output(logger->handler, logStr);
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
    if (logger == NULL) {
        return;
    }
    if (logger->handler == NULL) {
        return;
    }

    log_filter  *it      = logger->filter;
    log_Handler *handler = logger->handler;

    while (it != NULL) {
        if (it->_dispose(it, logger->level, message)) {
            output_to_handler(logger, level, color, message);
            if (it->jump_out) {
                return;
            }
        }
        it = it->next;
    }
    output_to_handler(logger, level, color, message);
}

void loggingMessage(Logger     *logger,
                    log_level   level,
                    const char *file,
                    int         line,
                    const char *message,
                    ...) {
    Logger *_logger = NULL;
    if (logger == NULL) {
        if (ROOT_LOGGER == NULL) {
            ROOT_LOGGER = loggingNewLogger("ROOT"); // 创建根日志对象
        }
        _logger = ROOT_LOGGER;
    } else {
        _logger = logger; // 使用传入的日志对象
    }

    if (_logger->level >= level) {
        char logStr
            [LOG_BUFFER_SIZE]; // 定义一个缓冲区，用于存储格式化后的日志字符串
        char    finalLogStr[LOG_BUFFER_SIZE *
                         2]; // 定义一个缓冲区，用于存储最终输出的日志字符串
        va_list args;
        va_start(args, message);
        vsprintf(logStr, message, args);
        va_end(args);
        snprintf(
            finalLogStr, LOG_BUFFER_SIZE * 2, "[%s:%d] %s", file, line, logStr);

        switch (level) {
        case LOG_DEBUG:
            log_cope(_logger, "Debug", BLUE, finalLogStr);
            break;
        case LOG_INFO:
            log_cope(_logger, "Info", GREEN, finalLogStr);
            break;
        case LOG_WARNING:
            log_cope(_logger, "Warning", YELLOW, finalLogStr);
            break;
        case LOG_ERROR:
            log_cope(_logger, "Error", RED, finalLogStr);
            break;
        case LOG_FATAL:
            log_cope(_logger, "Fatal", RED_B, finalLogStr);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief 创建一个日志句柄
 * @param name 日志器名称
 * @return 日志器对象
 */
Logger *loggingNewLogger(const char *name) {
    Logger *logger = (Logger *)malloc(sizeof(Logger));
    if (logger == NULL) {
        return NULL;
    }

    if (LOGGER_MAP == NULL) {
        LOGGER_MAP = map_create(sizeof(Logger **));
    }

    logger->level   = LOG_INFO;
    logger->handler = loggingHandlerConsole();
    logger->name    = name;
    logger->filter  = NULL;

    map_put(LOGGER_MAP, name, &logger);

    return logger;
}

Logger *loggingGetDefaultLogger(void) {
    if (ROOT_LOGGER != NULL) {
        return ROOT_LOGGER;
    }

    ROOT_LOGGER = loggingNewLogger("ROOT");
    return ROOT_LOGGER;
}

/**
 * @brief 获取日志器对象
 * @param name 日志器名称
 * @param level 日志等级
 * @return 日志器对象
 */
Logger *loggingGetLogger(const char *name) {
    if (name == NULL) {
        return NULL;
    }
    if (LOGGER_MAP == NULL) {
        LOGGER_MAP = map_create(sizeof(Logger **));
    }

    Logger **cache_logger_ptr = (Logger **)map_get(LOGGER_MAP, name);
    if (cache_logger_ptr != NULL) {
        return *cache_logger_ptr;
    }

    Logger *logger  = (Logger *)malloc(sizeof(Logger));

    logger->level   = LOG_INFO;
    logger->handler = loggingHandlerConsole();
    logger->name    = name;
    logger->filter  = NULL;

    map_put(LOGGER_MAP, name, &logger);

    return logger;
}

void loggingDestroyLogger(Logger *logger) {
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

static void
__destroyLoggerForeach(const char *key, void *value, void *user_data) {
    (void)user_data;
    (void)key;
    loggingDestroyLogger(*(Logger **)value);
}

/**
 * @brief 销毁日志对象
 */
void loggingDestroyAll(void) {
    map_foreach(LOGGER_MAP, __destroyLoggerForeach, NULL);
    map_destroy(LOGGER_MAP);
}