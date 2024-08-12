#include "logging.h"


int main() {
    // Your code goes here
    Logging *log = createLogging();
    Logger *logger = log->getLogger("testLogger",LOG_DEBUG);
    // log->addHandler(consoleHandler("test"));
    log->addHandler(fileHandler("test"));
    logger->info("This is an info message");
    logger->error("你好,这是一个错误消息%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogging(log);
    return 0;
}