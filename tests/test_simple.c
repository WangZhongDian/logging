#include "logging.h"

int main() {
    Logging *log    = newLogging();
    Logger  *logger = log->getLogger("testLogger", LOG_DEBUG);

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    log->destroyLogging(log);
    return 0;
}