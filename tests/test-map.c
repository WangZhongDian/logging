#include "utils/logging-map.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Map *map           = map_create(0, sizeof(int));

    const char *keys[] = {"key1", "key2", "key3", "key4", "key5"};

    for (int i = 0; i < 5; i++) {
        map_put(map, keys[i], &i);
    }

    map_destroy(map);
    return 0;
}