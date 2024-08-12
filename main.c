#include "logging.h"


int main() {
    // Your code goes here
    Logging *log = createLogging();
    Logger *logger = log->getLogger("testLogger",LOG_INFO);
    logger->info("This is an info message");
    logger->error("This is an error message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message");

    return 0;
}