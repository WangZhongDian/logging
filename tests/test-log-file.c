#include "logging.h"
#include "logging/logging-handler.h"

int main() {
    Logger      *logger = newDefaultLogger("testLogger", LOG_DEBUG);
    log_Handler *hander = loggingHandlerFile("test_log", 1024 * 1024 * 10);
    logger->addHandler(hander);

    Log_info("This is an info message");
    Log_error("This is an error message%s", "123");
    Log_fatal("This is an fatal message");
    Log_debug("This is a debug message");
    Log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}