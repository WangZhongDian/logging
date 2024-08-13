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
    fclose(handler->out);
    free(handler);
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
    handler->out = fp;
    handler->apply_color = false;
    handler->_free = __freeFileHandler;
    return handler;
}


static void __freeConsoleHandler(log_Handler* handler){
    free(handler);
}


/**
* @description ：控制台日志处理器
* @param 
* @return 
*/
log_Handler* consoleHandler(const char* name){
    log_Handler* handler = (log_Handler*)malloc(sizeof(log_Handler));
    handler->out = stdout;
    handler->apply_color = true;
    handler->_free = __freeConsoleHandler;
    return handler;
}