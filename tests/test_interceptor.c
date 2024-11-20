#include "logging.h"
#include <stdio.h>

int main() {
    Logger  *logger = newLogger("testLogger", LOG_DEBUG);

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    char *test1[]         = {"123", "tt"};

    log_Interceptor *tint = loggingSubStringInterceptor(
        test1,
        2,
        LOG_DEBUG,
        loggingFileHandler("test_interceptor", 1024 * 1024));

    logger->addInterceptor(tint);

    printf("\n");
    printf("Interceptor added\n");
    printf("\n");

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogger();
    return 0;
}