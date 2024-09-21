#include "logging.h"
#include <stdio.h>

int main() {
    Logging *log    = newLogging();
    Logger  *logger = log->getLogger("testLogger", LOG_DEBUG);

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    char *test1[] = {"123", "你好"}; // 要拦截的字符串
    // 添加拦截器，将拦截到的日志重定向到拦截器的专属处理器中
    log_Interceptor *tint =
        loggingSubStringInterceptor(test1, 2, LOG_DEBUG, loggingFileHandler("被拦截"));

    logger->addInterceptor(tint);

    printf("\n");
    printf("Interceptor added\n");
    printf("\n");

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    log->destroyLogging(log);
    return 0;
}