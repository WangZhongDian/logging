#include "logging.h"
#include "logging/logging-core.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int main() {
    Logger *logger = newDefaultLogger("testLogger", LOG_DEBUG);

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    char *test1[]         = {"This", NULL};

    log_Interceptor *tint = loggingSubStringInterceptor(
        test1,
        LOG_DEBUG,
        loggingFileHandler("test_interceptor", 1024 * 1024),
        false);

    logger->addInterceptor(tint);

    char *test2[]          = {"123", NULL};

    log_Interceptor *tint1 = loggingSubStringInterceptor(
        test2,
        LOG_ERROR,
        loggingFileHandler("test_interceptor1", 1024 * 1024),
        true);

    logger->addInterceptor(tint1);

    printf("\n");
    printf("Interceptor added\n");
    printf("\n");

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}