#include "logging/logging-filter.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct keywords_s {
    char              *key;
    struct keywords_s *next;
} keywords_t;

static void get_next(char *str, int *next) {
    next[1] = 0;
    int i   = 1;
    int j   = 0;
    while (i < strlen(str)) {
        if (j == 0 || str[i] == str[j]) {
            next[++i] = ++j;
        } else {
            j = next[j];
        }
    }
}

static bool kmp_search(char *substr, char *master) {
    if (substr == NULL)
        return true; // 空串全匹配
    if (master == NULL)
        return false;
    int  i         = 0;
    int  j         = 0;
    int  substrlen = strlen(substr);
    int  masterlen = strlen(master);
    int *next      = (int *)malloc(sizeof(int) * (substrlen + 1));
    get_next(substr, next);

    while (i < masterlen && j < substrlen) {
        if (master[i] == substr[j]) {
            i++;
            j++;
        } else {
            if (j == 0) {
                i++;
            } else {
                j = next[j];
            }
        }
    }

    free(next);
    if (j == substrlen)
        return true;
    else
        return false;
}

static bool _disposeSubstring(log_filter *filter,
                              log_level   level,
                              const char *message,
                              ...) {
    int         count   = 0;
    keywords_t *keyword = (keywords_t *)(filter + 1);

    if (keyword->key == NULL && keyword->next == NULL) {
        if (level <= filter->level)
            return true;
        return false;
    }

    while (keyword != NULL && level <= filter->level) {
        if (kmp_search(keyword->key, (char *)message))
            return true;
        keyword = keyword->next;
    }

    return false;
}

static void _freeFilter(log_filter *filter) {
    keywords_t *it_keyword =
        (keywords_t *)(filter + 1); // it_keyword 不是起始地址，请勿free
    keywords_t *keyword = it_keyword->next;
    keywords_t *next    = NULL;

    while (keyword != NULL) {
        next = keyword->next;
        free(keyword->key);
        free(keyword);
        keyword = next;
    }

    if (filter->handler != NULL) {
        filter->handler->_free(filter->handler);
    }

    if (it_keyword->key != NULL)
        free(it_keyword->key);
    free(filter);
}

log_filter *loggingFilterSubStr(char        *keywords[],
                                log_level    level,
                                log_Handler *handler,
                                bool         jump_out) {
    // 分配log_filter和keywords_t的连续内存
    log_filter *filter =
        (log_filter *)malloc(sizeof(log_filter) + sizeof(keywords_t));
    filter->_dispose    = _disposeSubstring;
    filter->handler     = handler;
    filter->level       = level;
    filter->jump_out    = jump_out;
    filter->_free       = _freeFilter;

    keywords_t *keyword = (keywords_t *)(filter + 1);
    keyword->key        = NULL;
    int count           = 0;
    if (keywords[count] != NULL) {
        keyword->key = strdup(keywords[count]);
        count++;
        keyword->next = NULL;
    }

    while (keywords[count] != NULL) {
        keyword->next = (keywords_t *)malloc(sizeof(keywords_t));
        keyword       = keyword->next;
        keyword->key  = strdup(keywords[count]);
        count++;
    }
    keyword->next = NULL;

    return filter;
}