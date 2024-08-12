
#include "logging.h"






log_Handler* fileHandler(const char* name,log_level level){
    FILE* fp = fopen(name, "w");
    log_Handler* handler = (log_Handler*)malloc(sizeof(log_Handler));
    handler->file = fp;
    handler->level = level;
    
    return handler;
}