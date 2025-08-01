#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "tracker.h"
#include "hash_table.h"
#define TABLE_SIZE 1000

void* track_malloc(size_t size) {
    if (size == 0) {
        printf("Warning: malloc called with size 0\n");
        return NULL;
    }

    void* ptr = malloc(size);
    if (!ptr) {
        printf("Error: malloc failed!\n");
        return NULL;
    }

    insert_record(ptr, size);
    return ptr;
}

void* track_calloc(size_t num, size_t size) {
    if (num == 0 || size == 0) {
        printf("Warning: calloc called with 0 size\n");
        return NULL;
    }

    void* ptr = calloc(num, size);
    if (!ptr) {
        printf("Error: calloc failed!\n");
        return NULL;
    }

    insert_record(ptr, num * size);
    return ptr;
}

void* track_realloc(void* ptr, size_t new_size) {
    if (new_size == 0) {
        printf("Warning: realloc called with 0 size. Treated as free().\n");
        track_free(ptr);
        return NULL;
    }

    if (ptr) {
        remove_record(ptr);
    }

    void* new_ptr = realloc(ptr, new_size);
    if (!new_ptr) {
        printf("Error: realloc failed!\n");
        return NULL;
    }

    insert_record(new_ptr, new_size);
    return new_ptr;
}

void track_free(void* ptr) {
    if (!ptr) {
        printf("Warning: Attempt to free NULL pointer.\n");
        return;
    }

    if (!remove_record(ptr)) {
        printf("Warning: Attempt to free untracked or already freed pointer %p\n", ptr);
        return;
    }

    free(ptr);
}

void free_all_tracked_memory() {
    HashNode** table = get_all_records();  // Get the hash table

    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* node = table[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;

            // Free the actual memory block if not already freed
            if (temp->address != NULL) {
                free(temp->address);
                temp->address = NULL;
            }

            free(temp);  // Free the HashNode itself
        }
        table[i] = NULL;
    }
}


void memoryLeakRemover(){
     free_all_tracked_memory();
}

void print_memory_log_to_console_and_logfile() {
    FILE* logFile = fopen("live_log.txt", "a");  // Append mode to preserve history
    if (!logFile) {
        printf("Error: Unable to write to live_log.txt\n");
        return;
    }

    // Get current time
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    // Header
    printf("\n==== Live Memory Tracker [%s] ====\n", time_str);
    fprintf(logFile, "\n==== Live Memory Tracker [%s] ====\n", time_str);

    HashNode** table = get_all_records();
    int count = 0;
    size_t total = 0;

    for (int i = 0; i < 1000; ++i) {
        HashNode* node = table[i];
        while (node) {
            printf("Address: %p | Size: %zu bytes\n", node->address, node->size);
            fprintf(logFile, "Address: %p | Size: %zu bytes\n", node->address, node->size);
            total += node->size;
            count++;
            node = node->next;
        }
    }

    if (count == 0) {
        printf("No current memory allocations.\n");
        fprintf(logFile, "No current memory allocations.\n");
    } else {
        printf("Total blocks: %d | Total memory: %zu bytes\n", count, total);
        fprintf(logFile, "Total blocks: %d | Total memory: %zu bytes\n", count, total);
    }

    printf("==================================\n");
    fprintf(logFile, "==================================\n");

    fclose(logFile);
}

void print_memory_log_to_console() {

    // Get current time
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    // Header
    printf("\n==== Currently Allocated Memory [%s] ====\n", time_str);

    HashNode** table = get_all_records();
    int count = 0;
    size_t total = 0;

    for (int i = 0; i < 1000; ++i) {
        HashNode* node = table[i];
        while (node) {
            printf("Address: %p | Size: %zu bytes\n", node->address, node->size);
            total += node->size;
            count++;
            node = node->next;
        }
    }

    if (count == 0) {
        printf("No current memory allocations.\n");
    } else {
        printf("Total blocks: %d | Total memory: %zu bytes\n", count, total);
    }

    printf("==================================\n");
}

void write_memory_log(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "==== Memory Leak Report ====\n");

    int leak_count = 0;
    size_t total_leaked = 0;

    HashNode** table = get_all_records();
    for (int i = 0; i < 1000; ++i) {
        HashNode* node = table[i];
        while (node) {
            fprintf(file, "LEAK: Address: %p | Size: %zu bytes\n", node->address, node->size);
            total_leaked += node->size;
            leak_count++;
            node = node->next;
        }
    }

    if (leak_count == 0)
        fprintf(file, "No memory leaks detected.\n");
    else
        fprintf(file, "Total leaks: %d | Total bytes leaked: %zu\n", leak_count, total_leaked);

    fclose(file);
}

void cleanup_memory_tracker() {
    write_memory_log("memory_log.txt");
    free_all_tracked_memory();
}
