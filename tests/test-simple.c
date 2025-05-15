#include "logging.h"

int main() {
    Logger *logger = newDefaultLogger("testLogger", LOG_DEBUG);

    Log_info("This is an info message");
    Log_error("This is an error message%s", "123");
    Log_fatal("This is an fatal message");
    Log_debug("This is a debug message");
    Log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}