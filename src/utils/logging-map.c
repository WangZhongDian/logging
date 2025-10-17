#include "logging-map.h"

#include <stdlib.h>
#include <string.h>

/* 哈希函数：FNV-1a */
static uint32_t hash_bytes(const void *data, size_t len) {
    const uint8_t *p = data;
    uint32_t       h = 2166136261u;
    while (len--) {
        h ^= *p++;
        h *= 16777619u;
    }
    return h;
}

/* 创建空 map */
Map *map_create(size_t key_len, size_t value_len) {
    Map *m        = calloc(1, sizeof(*m));
    m->key_len    = key_len;
    m->value_len  = value_len;
    m->bucket_cap = 8; /* 初始桶数 */
    m->bucket     = calloc(m->bucket_cap, sizeof(Node *));
    return m;
}

/* 根据 key 找到桶下标 */
static size_t bucket_index(Map *m, const void *key) {
    uint32_t h = hash_bytes(key, m->key_len);
    return h & (m->bucket_cap - 1); /* 要求 bucket_cap 是 2 的幂 */
}

/* 在桶链中线性查找 */
static Node *find_in_chain(Node *head, const void *key, size_t key_len) {
    for (; head; head = head->next) {
        if (memcmp(head->kv, key, key_len) == 0) {
            return head;
        }
    }
    return NULL;
}

/* 扩容：2× 桶数，重新哈希所有节点 */
static void map_resize(Map *m) {
    size_t new_cap    = m->bucket_cap * 2;
    Node **new_bucket = calloc(new_cap, sizeof(Node *));

    for (size_t i = 0; i < m->bucket_cap; ++i) {
        Node *node = m->bucket[i];
        while (node) {
            Node  *next     = node->next;
            size_t idx      = hash_bytes(node->kv, m->key_len) & (new_cap - 1);
            node->next      = new_bucket[idx];
            new_bucket[idx] = node;
            node            = next;
        }
    }
    free(m->bucket);
    m->bucket     = new_bucket;
    m->bucket_cap = new_cap;
}

/* 插入或覆盖 */
void map_put(Map *m, const void *key, const void *value) {
    if (m->size * 4 >= m->bucket_cap * 3) { /* 装载因子 0.75 */
        map_resize(m);
    }

    size_t idx  = bucket_index(m, key);
    Node  *node = find_in_chain(m->bucket[idx], key, m->key_len);

    if (node) { /* 覆盖旧值 */
        memcpy(node->kv + m->key_len, value, m->value_len);
        return;
    }

    /* 新建节点 */
    node       = malloc(sizeof(*node));
    node->kv   = malloc(m->key_len + m->value_len);
    node->next = m->bucket[idx];
    memcpy(node->kv, key, m->key_len);
    memcpy(node->kv + m->key_len, value, m->value_len);
    m->bucket[idx] = node;
    ++m->size;
}

/* 查找 */
void *map_get(Map *m, const void *key) {
    size_t idx  = bucket_index(m, key);
    Node  *node = find_in_chain(m->bucket[idx], key, m->key_len);
    return node ? (node->kv + m->key_len) : NULL;
}

/* 删除 */
bool map_erase(Map *m, const void *key) {
    size_t idx  = bucket_index(m, key);
    Node **link = &m->bucket[idx];
    for (; *link; link = &(*link)->next) {
        if (memcmp((*link)->kv, key, m->key_len) == 0) {
            Node *to_del = *link;
            *link        = to_del->next;
            free(to_del->kv);
            free(to_del);
            --m->size;
            return true;
        }
    }
    return false;
}

/* 销毁 */
void map_destroy(Map *m) {
    for (size_t i = 0; i < m->bucket_cap; ++i) {
        Node *node = m->bucket[i];
        while (node) {
            Node *next = node->next;
            free(node->kv);
            free(node);
            node = next;
        }
    }
    free(m->bucket);
    free(m);
}