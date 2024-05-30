#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void usage(const char *prog_name) {
    printf("Usage: %s -s <size_in_MB> [-r | -v]\n", prog_name);
    printf("  -s <size_in_MB>   Specify the size of memory to allocate in megabytes\n");
    printf("  -r                Claim memory as resident memory\n");
    printf("  -v                Claim memory as virtual memory\n");
}

size_t parse_size(const char *size_str) {
    size_t size;
    char unit = 'M'; // Default unit is megabytes if not specified

    if (sscanf(size_str, "%zu%c", &size, &unit) < 1) {
        fprintf(stderr, "Invalid size format: %s\n", size_str);
        exit(EXIT_FAILURE);
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
            exit(EXIT_FAILURE);
    }

    return size;
}

void allocate_memory(size_t allocation_size, int resident) {
    void *ptr = malloc(allocation_size);
    if (!ptr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    if (resident) {
        memset(ptr, 0, allocation_size);
        printf("Allocated %zu bytes of resident memory\n", allocation_size);
    } else {
        printf("Allocated %zu bytes of virtual memory\n", allocation_size);
    }

    // Sleep to keep the memory allocated for observation
    sleep(10);
}

int main(int argc, char *argv[]) {
    int opt;
    size_t allocation_size = 0;
    int mode = 0; // 1 for resident, 2 for virtual

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "s:rv")) != -1) {
        switch (opt) {
            case 's':
                allocation_size = parse_size(optarg);
                break;
            case 'r':
                mode = 1;
                break;
            case 'v':
                mode = 2;
                break;
            default:
                usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (allocation_size == 0 || mode == 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Allocate memory based on the selected mode
    if (mode == 1) {
        allocate_memory(allocation_size, 1);
    } else if (mode == 2) {
        allocate_memory(allocation_size, 0);
    }

    return EXIT_SUCCESS;
}

