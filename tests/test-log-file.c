#include "logging.h"
#include "logging/logging-handler.h"

int main() {
    Logger      *logger = newDefaultLogger(__FILE__, LOG_DEBUG);
    log_Handler *hander = loggingHandlerFile("test_log", 1024 * 1024 * 10);
    logger->addHandler(hander);

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}