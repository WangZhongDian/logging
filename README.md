# C语言日志库logging

## 简介

logging是一个轻量级的简单易用C语言日志库，支持日志级别、日志格式、日志输出、日志文件等功能。

## 功能

- 支持日志级别：DEBUG、INFO、WARN、ERROR、FATAL
- 支持日志格式：时间戳、日志级别、日志内容
- 支持日志输出：控制台、文件
- 支持日志文件：自动创建、自动滚动、自动删除(未完成)

## 安装
- conan安装使用
```shell
conan create . --build=missing
```
- cmake安装使用
```shell
```

## 使用方法

### 控制台日志
```c
#include "logging.h"

int main() {
    Logger *logger = newLogger("testLogger", LOG_DEBUG);

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogger();
    return 0;
}
```

### 文件日志
```c
#include "logging.h"
#include "logging/logging-handler.h"

int main() {
    Logger *logger = newLogger("testLogger", LOG_DEBUG);
    logger->addHandler(loggingFileHandler("test1", 1024*1024));

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogger();
    return 0;
}
```

### 日志拦截器
> 支持添加自定义的拦截器， 目前内置了子串拦截器

> 拦截器的作用:可以将拦截到的日志重定向到拦截器的专属处理器中


#### 例子
将拦截到的日志重定向到专属文件处理器中
```c
#include "logging.h"
#include <stdio.h>

int main() {
    Logger  *logger = newLogger("testLogger", LOG_DEBUG);

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

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

    logger->info("This is an info message");
    logger->error("This is an error message%s", "123");
    logger->fatal("This is an fatal message");
    logger->debug("This is a debug message");
    logger->warning("This is a warning message%s", "123");

    destroyLogger();
    return 0;
}
```
![](docs/img/2024-09-21-11-44-25.png)
![](docs/img/2024-09-21-11-44-06.png)

