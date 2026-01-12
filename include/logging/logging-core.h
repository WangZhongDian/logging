#ifndef __LOGGING_CORE_H__
#define __LOGGING_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_FATAL = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
} log_level;

static const char *LOG_LEVEL_STR[] = {
    "FATAL",
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
};

typedef enum {
    L_ERROR,
    L_OK,
} log_status;

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_CORE_H__
