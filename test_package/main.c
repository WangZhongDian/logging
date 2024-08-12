#include "logging.h"


int main() {
    Logging *log = createLogging();
    Logger *logger = log->getLogger("testLogger",LOG_DEBUG);
    // logger->addHandler(fileHandler("test"));
    logger->addHandler(consoleHandler("test"));

    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    Logger *logger1 = getCurrentLogger();
    logger1->info("This is an info message from logger1");

    destroyLogging(log);
    return 0;
}