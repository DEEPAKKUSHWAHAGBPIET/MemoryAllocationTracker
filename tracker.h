#ifndef TRACKER_H
#define TRACKER_H
#include <stddef.h>
#include "hash_table.h"

void* track_malloc(size_t size);
void* track_calloc(size_t num, size_t size);
void* track_realloc(void* ptr, size_t new_size);
void  track_free(void* ptr);
unsigned long hash_ptr(void* ptr);
void print_memory_log_to_console_and_logfile(char *type);
void print_memory_log_to_console();
void  write_memory_log(const char* filename);
void  cleanup_memory_tracker();
void free_all_tracked_memory();
size_t get_size_for_address(void* ptr);
unsigned int hash(void* ptr);
void init_hash_table();
void memoryLeakRemover();

#endif
