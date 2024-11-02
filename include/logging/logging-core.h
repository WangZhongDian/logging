#ifndef __LOGGING_CORE_H__
#define __LOGGING_CORE_H__

typedef enum {
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} log_level;

typedef enum {
    L_ERROR,
    L_OK,
} log_status;

#endif // __LOGGING_CORE_H__
