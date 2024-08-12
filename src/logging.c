#include "logging.h"

Logger* G_LOGGER = NULL;

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"
#define CYAN "\033[0;36m"
#define MAGENTA "\033[0;35m"
#define WHITE "\033[0;37m"
#define BLACK "\033[0;30m"

#define LOG_BUFFER_SIZE 1024


static void getTimeStr(char * timeStr){
    time_t t = time(NULL);
    struct tm* p = localtime(&t);
    char  _timeStr[20];
    strftime(_timeStr, sizeof(_timeStr), "%Y-%m-%d %H:%M:%S", p);
    strcpy(timeStr, _timeStr);
}


static void addHandler(log_Handler* handler){
    if (G_LOGGER == NULL){
        return;
    }
    G_LOGGER->handler = handler;
}

static void _builtin_log(char* level, const char *color, const char* format, ...){
    if (G_LOGGER == NULL){
        return;
    }
    if (G_LOGGER->handler == NULL){
        return;
    }
    char timeStr[20];
    getTimeStr(timeStr);
    char logStr[LOG_BUFFER_SIZE];
    if (G_LOGGER->handler->apply_color) sprintf(logStr, "%s %s%s%s %s\n", timeStr, color,level,RESET, format);
    else sprintf(logStr, "%s %s %s\n", timeStr, level, format);
    fputs(logStr, G_LOGGER->handler->out);
}


//*************************记录日志******************************* */
static void fatal(const char* format, ...){
    if (G_LOGGER->level >= LOG_ERROR){
        char logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsprintf(logStr, format, args);
        va_end(args);
        _builtin_log("Fatal",RED, logStr, args);
    }
}

static void error(const char* format, ...){
    if (G_LOGGER->level >= LOG_ERROR){
        char logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsprintf(logStr, format, args);
        va_end(args);
        _builtin_log("Error",RED, logStr, args);
    }
}

static void warning(const char* format, ...){
    if (G_LOGGER->level >= LOG_WARNING){
        char logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsprintf(logStr, format, args);
        va_end(args);
        _builtin_log("Warning",YELLOW, logStr, args);
    }
}

static void info(const char* format, ...){
    if (G_LOGGER->level >= LOG_INFO){
        char logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsprintf(logStr, format, args);
        va_end(args);
        _builtin_log("Info",GREEN, logStr, args);
    }
}

static void debug(const char* format, ...){
    if (G_LOGGER->level >= LOG_DEBUG){
        char logStr[LOG_BUFFER_SIZE];
        va_list args;
        va_start(args, format);
        vsprintf(logStr, format, args);
        va_end(args);
        _builtin_log("Debug",CYAN, logStr, args);
    }
}
//*************************记录日志******************************* */


/**
* @description :获取一个日志操作对象
* @param 
* @return 
*/
static Logger* getLogger(const char* name, log_level level){
    if (G_LOGGER != NULL){
        return G_LOGGER;
    }
    Logger* logger = (Logger*)malloc(sizeof(Logger));
    logger->fatal = fatal;
    logger->error = error;
    logger->warning = warning;
    logger->info = info;
    logger->debug = debug;

    logger->level = level;
    logger->handler = NULL;
    logger->name = name;

    G_LOGGER = logger;
    return G_LOGGER;
}

/**
* @description :创建一个日志对象
* @return :Logging* 返回一个日志对象
*/
Logging* createLogging(){
    Logging* logging = (Logging*)malloc(sizeof(Logging));
    logging->getLogger = getLogger;
    logging->addHandler = addHandler;
    return logging;
}


log_status destroyLogging(Logging* logging){
    if (logging == NULL){
        return L_ERROR;
    }
    if (G_LOGGER != NULL){
        if (G_LOGGER->handler != NULL){
            fclose(G_LOGGER->handler->out);
            free(G_LOGGER->handler);
        }
        free(G_LOGGER);
        G_LOGGER = NULL;
    }
    free(logging);
    return L_OK;
}