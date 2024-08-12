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



static void getTimeStr(char * timeStr){
    time_t t = time(NULL);
    struct tm* p = localtime(&t);
    char _timeStr[18];
    strftime(_timeStr, sizeof(_timeStr), "%Y-%m-%d %H:%M:%S", p);
    strcpy(timeStr, _timeStr);
}


static void addHandler(log_Handler* handler){
    if (G_LOGGER == NULL){
        return;
    }
    G_LOGGER->handler = handler;
}


//*************************记录日志******************************* */
static void error(const char* format, ...){
    if (G_LOGGER->level >= LOG_ERROR){
        char timeStr[18];
        getTimeStr(timeStr);
        printf("%sError%s   %s %s\n",RED,RESET, timeStr, format);
    }

}

static void warning(const char* format, ...){
    if (G_LOGGER->level >= LOG_WARNING){
        char timeStr[18];
        getTimeStr(timeStr);
        printf("%sWarning%s %s %s\n",YELLOW,RESET, timeStr, format);
    }

}

static void info(const char* format, ...){
    if (G_LOGGER->level >= LOG_INFO){
        char timeStr[18];
        getTimeStr(timeStr);
        printf("%sInfo%s    %s %s\n",GREEN,RESET, timeStr, format);
    }
}

static void debug(const char* format, ...){
    if (G_LOGGER->level >= LOG_DEBUG){
        char timeStr[18];
        getTimeStr(timeStr);
        printf("%sDebug%s   %s %s\n",CYAN,RESET, timeStr, format);
    }
}
//*************************记录日志******************************* */


/**
* @description :获取一个日志操作对象
* @param 
* @return 
*/
static Logger* getLogger(const char* name, log_level level){
    Logger* logger = (Logger*)malloc(sizeof(Logger));
    logger->error = error;
    logger->warning = warning;
    logger->info = info;
    logger->debug = debug;

    logger->level = level;
    logger->handler = NULL;

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