#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

#define TABLE_SIZE 1000

static HashNode* hash_table[TABLE_SIZE];

unsigned long hash_ptr(void* ptr) {
    return ((unsigned long)ptr >> 3) % TABLE_SIZE;
}

void init_hash_table() {
    memset(hash_table, 0, sizeof(hash_table));
}

void insert_record(void* address, size_t size) {
    unsigned long idx = hash_ptr(address);
    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->address = address;
    new_node->size = size;
    new_node->next = hash_table[idx];
    hash_table[idx] = new_node;
}

int remove_record(void* address) {
    unsigned long idx = hash_ptr(address);
    HashNode* curr = hash_table[idx], *prev = NULL;

    while (curr) {
        if (curr->address == address) {
            if (prev) prev->next = curr->next;
            else hash_table[idx] = curr->next;
            free(curr);
            return 1;  // Found and removed
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;  // Not found
}

HashNode** get_all_records() {
    return hash_table;
}

int has_leak() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* node = hash_table[i];
        if (node != NULL) {
            return 1;  // Memory still allocated
        }
    }
    return 0;  // No leaks
}

void free_hash_table() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* node = hash_table[i];
        while (node) {
            HashNode* tmp = node;
            node = node->next;
            free(tmp);
        }
        hash_table[i] = NULL;
    }
}
