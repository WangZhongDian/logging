
#include "logging.h"


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
    return handler;
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
    return handler;
}