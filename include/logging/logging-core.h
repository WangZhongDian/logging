#ifndef __LOGGING_CORE_H__
#define __LOGGING_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
} log_level;

typedef enum {
    L_ERROR,
    L_OK,
} log_status;

#ifdef __cplusplus
}
#endif

#endif // __LOGGING_CORE_H__
