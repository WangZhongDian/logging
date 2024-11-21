# C language logging library logging

## brief

Logging is a lightweight and easy-to-use C language log library that supports log level, log format, log output, log files, and other functions.

## function
- Support log levels: DEBUG, INFO, Warning, ERROR, FATAL
- Support log formats: timestamp, log level, log content
- Support log output: console, file
- Support log files: automatic creation, automatic scrolling, log segmentation

## install
- Conan
```shell
conan create .
```
- cmake
```shell
```

## usage

### console log 
```c
#include "logging.h"

int main() {
    Logger *logger = newDefaultLogger("testLogger", LOG_DEBUG);

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}
```

### file log
```c
#include "logging.h"
#include "logging/logging-handler.h"

int main() {
    Logger *logger = newDefaultLogger("testLogger", LOG_DEBUG);
    logger->addHandler(loggingFileHandler("test1", 1024*1024));

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}
```

### Logging Interceptor 
> Support adding custom interceptors, currently with built-in substring interceptors
> The function of an interceptor is to redirect intercepted logs to the interceptor's dedicated processor


#### example
Redirects intercepted logs to a dedicated file processor
```c
#include "logging.h"
#include <stdio.h>

int main() {
    Logger  *logger = newDefaultLogger("testLogger", LOG_DEBUG);

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

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

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    destroyDefaultLogger();
    return 0;
}
```
![](docs/img/2024-09-21-11-44-25.png)
![](docs/img/2024-09-21-11-44-06.png)

