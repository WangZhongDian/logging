#include "logging.h"


int main() {
    Logging *log = createLogging();
    Logger *logger = log->getLogger("testLogger",LOG_DEBUG);
    logger->addHandler(consoleHandler("test"));

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    char *test1[] = {"123", "你好"};//要拦截的字符串
    //添加拦截器，将拦截到的日志重定向到拦截器的专属处理器中
    log_Interceptor * tint = substringInterceptor(test1,2,LOG_DEBUG,fileHandler("被拦截")); 
    logger->addInterceptor(tint);
    printf("Interceptor added\n");

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");

    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogging(log);
    return 0;
}