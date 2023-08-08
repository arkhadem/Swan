#include "swan.hpp"
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>

long *pollute_cache(size_t size) {
    const size_t bigger_than_cachesize = size / sizeof(long);
    long *p = (long *)malloc(size);
    for (int i = 0; i < bigger_than_cachesize; i++) {
        p[i] = i;
    }
    return p;
}

#define PLATFORM_IDX 1
#define LIBRARY_IDX 2
#define KERNEL_IDX 3
#define ROUNDS_IDX 4
#define EXECUTE_IDX 5
#define MAX_IDX 6

bool argc_has(int argc, int idx) {
    return (argc > idx);
}

int main(int argc, char *argv[]) {
    char *library;
    char *kernel;
    int rounds = -1;
    bool execute = true;
    platform_t platform = platform_t::SCALAR_PLAT;
    if (argc_has(argc, KERNEL_IDX) == false) {
        printf("missing argument: <platform: scalar|neon|compare> <library> <kernel> [ <rounds [<execute=false|true>] ]\n");
        exit(-1);
    }
    if (strcmp(argv[PLATFORM_IDX], "scalar") == 0) {
        platform = platform_t::SCALAR_PLAT;
    } else if (strcmp(argv[PLATFORM_IDX], "neon") == 0) {
        platform = platform_t::NEON_PLAT;
    } else if (strcmp(argv[PLATFORM_IDX], "compare") == 0) {
        platform = platform_t::MAX_PLAT;
    } else {
        printf("Error: platform must be either \"scalar\" or \"neon\" or \"compare\". Unrecognized \"%s\"\n", argv[PLATFORM_IDX]);
        exit(-1);
    }
    library = argv[LIBRARY_IDX];
    kernel = argv[KERNEL_IDX];

    if (argc_has(argc, ROUNDS_IDX) == true) {
        rounds = atoi(argv[ROUNDS_IDX]);
        if (rounds <= 0) {
            printf("Error: rounds (%s) must be > 0\n", argv[ROUNDS_IDX]);
            exit(-1);
        }
    }
    if (argc_has(argc, EXECUTE_IDX) == true) {
        if (strcmp(argv[EXECUTE_IDX], "true") == 0) {
            execute = true;
        } else if (strcmp(argv[EXECUTE_IDX], "false") == 0) {
            execute = false;
        } else {
            printf("Error: execute must be either \"true\" or \"false\". Unrecognized \"%s\"\n", argv[EXECUTE_IDX]);
            exit(-1);
        }
    }
    if (argc_has(argc, MAX_IDX) == true) {
        printf("Error: at most %d arguments are acceptable.\n", MAX_IDX);
        exit(-1);
    }
    benchmark_runner(platform, library, kernel, rounds, execute);
    return 0;
}