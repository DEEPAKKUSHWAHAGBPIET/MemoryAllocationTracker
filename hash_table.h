#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

typedef struct HashNode {
    void* address;
    size_t size;
    struct HashNode* next;
} HashNode;

void init_hash_table();
void insert_record(void* address, size_t size);
int remove_record(void* address);
void free_all_tracked_memory();  // Frees everything still tracked
HashNode** get_all_records();
void free_hash_table();
int has_leak();

#endif
