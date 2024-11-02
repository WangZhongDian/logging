#include "logging.h"
#include "logging/logging-handler.h"
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

static void getTimeStr(char *timeStr) {
    time_t     t = time(NULL);
    struct tm *p = localtime(&t);
    char       _timeStr[20];
    strftime(_timeStr, sizeof(_timeStr), "%Y-%m-%d %H:%M:%S", p);
    strcpy(timeStr, _timeStr);
}

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
                "%s: %s %s%s%s %s\n",
                G_LOGGER->name,
                timeStr,
                color,
                level,
                RESET,
                message);
    else
        sprintf(
            logStr, "%s: %s %s %s\n", G_LOGGER->name, timeStr, level, message);

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

static Logger *getLogger(const char *name, log_level level) {
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

Logger *getCurrentLogger(void) {
    if (G_LOGGER == NULL) {
        return NULL;
    }
    return G_LOGGER;
}

Logging *newLogging() {
    Logging *logging          = (Logging *)malloc(sizeof(Logging));
    logging->getLogger        = getLogger;
    logging->destroyLogging   = destroyLogging;
    logging->getCurrentLogger = getCurrentLogger;
    return logging;
}