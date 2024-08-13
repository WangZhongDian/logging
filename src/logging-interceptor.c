/********************************************
* @Date: 2024 08 12
* @Description: 日志拦截器
*********************************************/  

#include "logging.h"

static log_Interceptor* G_interceptor = NULL;

static char **G_keywords = NULL;

static void get_next(char *str, int *next) {
    next[1] = 0;
    int i=1;
    int j=0;
    while (i < strlen(str)) {
        if (j==0|| str[i] == str[j]) {
            next[++i] = ++j;
        }
        else {
            j = next[j];
        }
    }
}


static bool kmp_search(char *substr, char *master) {
    int i = 0;
    int j = 0;
    int substrlen = strlen(substr);
    int masterlen = strlen(master);
    int *next = (int *)malloc(sizeof(int) * substrlen + 1);
    get_next(substr, next);

    while (i < masterlen && j < substrlen) {
        if (master[i] == substr[j]) {
            i++;
            j++;
        } else {
            if(j == 0) {
                i++;
            } else {
                j = next[j];
            }
        }
    }

    free(next);
    if (j == substrlen)return true;
    else return false;
}


static bool _disposeSubstring(char* level,const char *message, ...){
    int count = 0;

    if (G_keywords == NULL) {
        return false;
    }

    while (G_keywords[count] != NULL)
    {
        if (kmp_search(G_keywords[count],(char*)message)) {
            if(G_interceptor->handler != NULL) {

            }
            return true;
        }
        count++;
    }

    return false;
}

/**
* @description : 完成拦截器自我释放内存
*/
static void _freeSubstring(log_Interceptor* interceptor) {
    int sum = 0;
    while (G_keywords[sum] != NULL) {
        free(G_keywords[sum]);
        sum++;
    }
    free(G_keywords);
    G_keywords = NULL;

    if(interceptor->handler != NULL){
        interceptor->handler->_free(interceptor->handler);
    }

    free(interceptor);
}

/**
* @description : 子字符串拦截器
*/
log_Interceptor* substringInterceptor(char *keywords[], int count, log_level level, log_Handler* handler) {
    log_Interceptor* interceptor = (log_Interceptor*)malloc(sizeof(log_Interceptor));
    interceptor->_dispose = _disposeSubstring;
    interceptor->handler = handler;
    interceptor->level = level;
    interceptor->_free = _freeSubstring;

    G_keywords = (char**)malloc( (sizeof(G_keywords)*count) + 1);

    for (int i = 0; i < count; i++) {
        G_keywords[i] = (char*)malloc(strlen(keywords[i])+1);
        strcpy(G_keywords[i], keywords[i]);
    }
    G_keywords[count] = NULL;

    G_interceptor = interceptor;
    return G_interceptor;
}



