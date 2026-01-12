#include "utils/logging-map.h"
#include <stdio.h>
#include <string.h>

void foreach_callback(const char *key, void *value, void *user_data) {
    (void)user_data;
    printf("foreach key: %s, value: %d\n", key, *(int *)value);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Map *map           = map_create(sizeof(int));

    const char *keys[] = {"key1", "key22", "key33", "key44", "key55"};

    for (int i = 0; i < 5; i++) {
        printf("put key %s,value %d\n", keys[i], i);
        map_put(map, keys[i], &i);
    }

    for (int i = 0; i < 5; i++) {
        int *value = map_get(map, keys[i]);
        printf("get value: %d\n", *value);
        if (*value != i) {
            return 1;
        }
    }

    map_foreach(map, foreach_callback, NULL);

    map_destroy(map);

    return 0;
}
