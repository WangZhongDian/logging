/********************************************
* @Date: 2024 08 12
* @Description: 日志处理器
********************************************/
#include "logging.h"


/**
* @description ：释放文件日志处理器相关资源
* @param 
* @return 
*/
static void __freeFileHandler(log_Handler* handler){
    fclose(handler->stream);
    free(handler);
}

static void outputFileHandler(log_Handler *handler, const char * message){
    fputs(message,handler->stream);
}

/**
* @description ：文件日志处理器
* @param 
* @return 
*/
log_Handler* fileHandler(const char* name){
    char new_file_name[100];
    sprintf(new_file_name, "%s.log", name);
    FILE* fp = fopen(new_file_name, "at");
    log_Handler* handler = (log_Handler*)malloc(sizeof(log_Handler));
    handler->stream = fp;
    handler->apply_color = false;
    handler->_free = __freeFileHandler;
    handler->output = outputFileHandler;
    return handler;
}



static void __freeConsoleHandler(log_Handler* handler){
    free(handler);
}


static void outputConsoleHandler(log_Handler* handler,const char * message){
    fputs(message,handler->stream);
}

/**
* @description ：控制台日志处理器
* @param 
* @return 
*/
log_Handler* consoleHandler(const char* name){
    log_Handler* handler = (log_Handler*)malloc(sizeof(log_Handler));
    handler->stream = stdout;
    handler->apply_color = true;
    handler->_free = __freeConsoleHandler;
    handler->output = outputConsoleHandler;
    return handler;
}