#include "logging-utils.h"
#include <time.h>
#include <string.h>

/**
* @brief 获取当前时间字符串
* @param timeStr 存储时间字符串缓冲区指针
*/
void getTimeStr(char *timeStr) {
    time_t     t = time(NULL);
    struct tm *p = localtime(&t);
    char       _timeStr[20];
    strftime(_timeStr, sizeof(_timeStr), "%Y-%m-%d %H:%M:%S", p);
    strcpy(timeStr, _timeStr);
}