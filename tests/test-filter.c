#include "logging.h"
#include "logging/logging-core.h"
#include "logging/logging-filter.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int main() {
    Logger *logger = newDefaultLogger(__FILE__, LOG_DEBUG);

    Log_info("This is an info message");
    Log_error("This is an error message%s", "123");
    Log_fatal("This is an fatal message");
    Log_debug("This is a debug message");
    Log_warning("This is a warning message%s", "123");

    char *test1[] = {"This", NULL};

    log_filter *tint =
        loggingFilterSubStr(test1,
                            LOG_DEBUG,
                            loggingHandlerFile("test_interceptor", 1024 * 1024),
                            false);

    logger->addFilter(tint);

    char *test2[]     = {"123", NULL};

    log_filter *tint1 = loggingFilterSubStr(
        test2,
        LOG_ERROR,
        loggingHandlerFile("test_interceptor1", 1024 * 1024),
        true);

    logger->addFilter(tint1);

    printf("\n");
    printf("filter added\n");
    printf("\n");

    Log_info("This is an info message");
    Log_error("This is an error message%s", "123");
    Log_fatal("This is an fatal message");
    Log_debug("This is a debug message");
    Log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}