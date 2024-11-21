# C语言日志库logging

[English](README.en.md)

## 简介

logging是一个轻量级的简单易用C语言日志库，支持日志级别、日志格式、日志输出、日志文件等功能。

## 功能

- 支持日志级别：DEBUG、INFO、WARN、ERROR、FATAL
- 支持日志格式：时间戳、日志级别、日志内容
- 支持日志输出：控制台、文件
- 支持日志文件：自动创建、自动滚动、日志分割

## 安装
- conan安装使用
```shell
conan create .
```
- cmake安装使用
```shell
```

## 使用方法
![](docs/img/2024-09-21-11-44-25.png)
![](docs/img/2024-09-21-11-44-06.png)

### 控制台日志
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

### 文件日志
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

### 日志拦截器
> 支持添加自定义的拦截器， 目前内置了子串拦截器

> 拦截器的作用:可以将拦截到的日志重定向到拦截器的专属处理器中


#### 单个子串拦截器
将拦截到的日志重定向到专属文件处理器中
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

    char *test1[]         = {"123", "tt", NULL};

    log_Interceptor *tint = loggingSubStringInterceptor(
        test1,
        LOG_DEBUG,
        loggingFileHandler("test_interceptor", 1024 * 1024),
        true);

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

#### 多个子串拦截器
```c
#include "logging.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

int main() {
    Logger *logger = newDefaultLogger("testLogger", LOG_DEBUG);

    log_info("This is an info message");
    log_error("This is an error message%s", "123");
    log_fatal("This is an fatal message");
    log_debug("This is a debug message");
    log_warning("This is a warning message%s", "123");

    char *test1[]         = {"This",NULL};

    log_Interceptor *tint = loggingSubStringInterceptor(
        test1,
        LOG_DEBUG,
        loggingFileHandler("test_interceptor", 1024 * 1024),
        false);

    logger->addInterceptor(tint);

    char *test2[]         = {"123",NULL};

    log_Interceptor *tint1 = loggingSubStringInterceptor(
        test2,
        LOG_DEBUG,
        loggingFileHandler("test_interceptor1", 1024 * 1024),
        true);

    logger->addInterceptor(tint1);

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

