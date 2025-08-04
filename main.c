#include <stdio.h>
#include <stdlib.h>
#include "tracker.h"
#include "hash_table.h"

void print_menu() {
    printf("\n===== Memory Tracker Menu =====\n");
    printf("1. Allocate Memory (malloc)\n");
    printf("2. Allocate Memory (calloc)\n");
    printf("3. Reallocate Memory\n");
    printf("4. Free Memory\n");
    printf("5. View Current Allocations\n");
    printf("6. Free ALL allocated memory\n");
    printf("0. Exit\n");
    printf("==================================\n");
    printf("Enter your choice: ");
}

#define MAX_PTRS 100
void* user_ptrs[MAX_PTRS] = {0};  // To store addresses for user to manage

int main() {
    init_hash_table();
    atexit(cleanup_memory_tracker);  // Write leak log on exit

    int choice;
    int index;
    size_t size, count, newcount, newsize;
    void* ptr;
    char ch;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter size to malloc: ");
                scanf("%zu", &size);
                for (index = 0; index < MAX_PTRS; index++) {
                    if (user_ptrs[index] == NULL) break;
                }
                if (index == MAX_PTRS) {
                    printf("Pointer storage full!\n");
                    break;
                }
                ptr = track_malloc(size);
                user_ptrs[index] = ptr;
                printf("Memory allocated at ptr[%d] = %p\n", index, ptr);
                print_memory_log_to_console_and_logfile("Malloc");
                break;
               

            case 2:
                printf("Enter count and size: ");
                scanf("%zu %zu", &count, &size);
                for (index = 0; index < MAX_PTRS; index++) {
                    if (user_ptrs[index] == NULL) break;
                }
                if (index == MAX_PTRS) {
                    printf("Pointer storage full!\n");
                    break;
                }
                ptr = track_calloc(count, size);
                user_ptrs[index] = ptr;
                printf("Memory allocated at ptr[%d] = %p\n", index, ptr);
                print_memory_log_to_console_and_logfile("Calloc");
                break;

            case 3:{
                printf("Enter index to realloc: ");
                scanf("%d", &index);
                char choice;
                printf("is it malloc or calloc allocated? m/c: ");
                scanf(" %c", &choice);
                if (index >= MAX_PTRS || user_ptrs[index] == NULL) {
                    printf("Invalid index.\n");
                    break;
                }
                if(choice=='m' || choice=='M'){
                     printf("Enter new size: ");
                     scanf("%zu", &size);
                }
                else{
                    printf("Enter new count and size: ");
                    scanf("%zu %zu", &newcount, &newsize);
                    size = newcount*newsize;
                }
                
                user_ptrs[index] = track_realloc(user_ptrs[index], size);
                printf("Reallocated ptr[%d] to %p\n", index, user_ptrs[index]);
                print_memory_log_to_console_and_logfile("type");
                break;
          }

            case 4:
                printf("Enter index to free: ");
                scanf("%d", &index);
                if (index >= MAX_PTRS || user_ptrs[index] == NULL) {
                    printf("Invalid index.\n");
                    break;
                }
                track_free(user_ptrs[index]);
                user_ptrs[index] = NULL;
               //  printf("Memory freed at index %d.\n", index);
                break;

            case 5:
                print_memory_log_to_console();
                //print_memory_log_to_console_and_logfile("ok");
                break;
            case 6:
               free_all_tracked_memory();
               // Clear user_ptrs array
               for (int i = 0; i < MAX_PTRS; ++i) user_ptrs[i] = NULL;
               printf("All allocated memory is now deallocated...");
               break;

            case 0: {
                    if (has_leak()) {
                        printf("\n\033[1;31mMemory leak(s) detected!\033[0m\n\n");

                         char choice;
                         printf("Do you want to free all leaked memory before exiting? (y/n): ");
                         scanf(" %c", &choice);

                         if (choice == 'y' || choice == 'Y') {
                              cleanup_memory_tracker();  // Frees all and logs
                              printf("All memory successfully deallocated.\n");
                         } else {
                              printf("Generating final leak report...\n");
                              printf("\033[1;33mExiting without freeing leaked memory.\033[0m\n");
                         }
                    } else {
                         printf("No memory leak detected. Exiting cleanly.\n");
                    }

                    exit(0);
               }

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
