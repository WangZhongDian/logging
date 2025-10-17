#ifndef __LOGGING_MAP_H__
#define __LOGGING_MAP_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Node {
    struct Node *next;
    uint8_t     *kv; /* key 在前 value 在后 */
} Node;

struct Map {
    size_t key_len, value_len;
    size_t bucket_cap;
    size_t size;   /* 当前元素个数 */
    Node **bucket; /* 指针数组 */
};

typedef struct Map Map;

/* 创建空 map：key_len/value_len 以字节为单位 */
Map *map_create(size_t key_len, size_t value_len);

/* 插入或覆盖：<key> 必须指向 key_len 字节；同理 value */
void map_put(Map *m, const void *key, const void *value);

/* 查找：找到返回 value 指针；未找到返回 NULL */
void *map_get(Map *m, const void *key);

/* 删除：返回 true 表示确实删掉了 */
bool map_erase(Map *m, const void *key);

/* 销毁并释放所有内存 */
void map_destroy(Map *m);

#endif /* __LOGGING_MAP_H__ */