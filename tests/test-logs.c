#include "logging.h"
#include "logging/logging-core.h"
#include <stdio.h>

int main() {
    Logger *t1  = loggingGetLogger("Test1");
    t1->level   = LOG_ERROR;

    Logger *t2  = loggingGetLogger("Test2");
    t2->level   = LOG_DEBUG;

    Logger *t11 = loggingGetLogger("Test1");

    if (t1 == t11) {
        printf("t1 and t11 are the same\n");
        printf("t1 log level: %s", LOG_LEVEL_STR[t11->level]);
    } else {
        printf("t1 and t11 are different\n");
        return 1;
    }

    loggingDestroyAll();
    return 0;
}