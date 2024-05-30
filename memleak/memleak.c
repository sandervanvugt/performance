#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Function to display usage instructions
void usage(const char *prog_name) {
    printf("Usage: %s [-v | -r] -s <size>\n", prog_name);
    printf("  -v        Exhaust virtual memory\n");
    printf("  -r        Exhaust resident memory\n");
    printf("  -s <size> Specify chunk size (e.g., 128M for 128 MiB)\n");
}

// Function to parse the size string and convert it to bytes
size_t parse_size(const char *size_str) {
    size_t size;
    char unit;
    
    if (sscanf(size_str, "%zu%c", &size, &unit) != 2) {
        fprintf(stderr, "Invalid size format: %s\n", size_str);
        exit(1);
    }
    
    switch (unit) {
        case 'K': case 'k':
            size *= 1024;
            break;
        case 'M': case 'm':
            size *= 1024 * 1024;
            break;
        case 'G': case 'g':
            size *= 1024 * 1024 * 1024;
            break;
        default:
            fprintf(stderr, "Invalid size unit: %c\n", unit);
            exit(1);
    }
    
    return size;
}

// Function to exhaust virtual memory
void exhaust_virtual_memory(size_t allocation_size) {
    while (1) {
        void *ptr = malloc(allocation_size);
        if (!ptr) {
            perror("malloc");
            break;
        }
        printf("Allocated %zu bytes of virtual memory\n", allocation_size);
        sleep(1); // Slow down the allocation to observe the process
    }
}

// Function to exhaust resident memory
void exhaust_resident_memory(size_t allocation_size) {
    while (1) {
        void *ptr = malloc(allocation_size);
        if (!ptr) {
            perror("malloc");
            break;
        }
        memset(ptr, 0, allocation_size);
        printf("Allocated %zu bytes of resident memory\n", allocation_size);
        sleep(1); // Slow down the allocation to observe the process
    }
}

int main(int argc, char *argv[]) {
    int opt;
    size_t allocation_size = 0;
    int mode = 0; // 1 for virtual, 2 for resident
    
    // Parse command line arguments
    while ((opt = getopt(argc, argv, "vrs:")) != -1) {
        switch (opt) {
            case 'v':
                mode = 1;
                break;
            case 'r':
                mode = 2;
                break;
            case 's':
                allocation_size = parse_size(optarg);
                break;
            default:
                usage(argv[0]);
                return 1;
        }
    }
    
    if (mode == 0 || allocation_size == 0) {
        usage(argv[0]);
        return 1;
    }
    
    // Execute the appropriate memory exhaustion function
    if (mode == 1) {
        exhaust_virtual_memory(allocation_size);
    } else if (mode == 2) {
        exhaust_resident_memory(allocation_size);
    }
    
    return 0;
}

