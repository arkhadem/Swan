#include "swan.hpp"

#include "runner.hpp"
#include "utility.hpp"
#include <stddef.h>
#include <stdio.h>
#include <string>

std::map<std::string, std::map<std::string, std::map<platform_e, kernel_func>>> kernel_functions;

void list_libkers() {
    printf("Supported library: [kernels]\n");
    std::map<std::string, std::map<std::string, std::map<platform_e, kernel_func>>>::iterator lib_it = kernel_functions.begin();
    while (lib_it != kernel_functions.end()) {
        std::string lib_str = lib_it->first;
        std::map<std::string, std::map<platform_e, kernel_func>> kernels_map = (lib_it++)->second;
        printf("%s: [", lib_str.c_str());
        std::map<std::string, std::map<platform_e, kernel_func>>::iterator ker_it = kernels_map.begin();
        while (ker_it != kernels_map.end()) {
            std::string ker_str = (ker_it++)->first;
            printf("%s", ker_str.c_str());
            if (ker_it != kernels_map.end()) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

void benchmark_runner(platform_t platform, const char *library, const char *kernel, int iterations, bool execute, bool list) {
    register_utilities();
    register_scalar_kernels();
    register_neon_kernels();
    if (list) {
        list_libkers();
        return;
    }
    std::string lib_str = std::string(library);
    std::string ker_str = std::string(kernel);
    if (kernel_functions.find(lib_str) == kernel_functions.end()) {
        printf("Error: library \"%s\" does not exist!\n", library);
        exit(-1);
    }
    if (kernel_functions[lib_str].find(ker_str) ==
        kernel_functions[lib_str].end()) {
        printf("Error: kernel \"%s\" does not exist!\n", kernel);
        exit(-1);
    }
    kernel_utility_t utility_function = utility_functions[lib_str][ker_str];
    config_s *config = nullptr;
    int count = utility_function.config_init(sizeof(long) * CACHE_SIZE, config);

#if CACHE_STATUS == CACHE_STATUS_COLD
    long *tmp = pollute_cache(sizeof(long) * CACHE_SIZE);
    printf("Cache polluted (%ld) | Count (%d)\n", tmp[rand() % CACHE_SIZE], count);
#else
    count = 1;
#endif

    input_s **input = nullptr;

    if (platform == platform_t::MAX_PLAT) {
        output_s **output_scalar = nullptr;
        output_s **output_neon = nullptr;
        utility_function.input_init(1, config, input);
        utility_function.output_init(1, config, output_scalar);
        utility_function.output_init(1, config, output_neon);
        kernel_functions[lib_str][ker_str][platform_t::SCALAR_PLAT](config, input[0], output_scalar[0]);
#ifdef SWAN_SIMULATION
        char graph_name[100];
        sprintf(graph_name, "%s_%s_%d", library, kernel, SWAN_REG_TYPE);
        fake_neon_initializer(graph_name);
#endif
        kernel_functions[lib_str][ker_str][platform_t::NEON_PLAT](config, input[0], output_neon[0]);
#ifdef SWAN_SIMULATION
        fake_neon_finisher();
#endif
        utility_function.comparer(config, output_scalar[0], output_neon[0]);
    } else {
        kernel_func kernel_func = kernel_functions[lib_str][ker_str][platform];
        output_s **output = nullptr;
        utility_function.input_init(count, config, input);
        utility_function.output_init(count, config, output);
        int idx = 0;
        // in us
        double time_spent = 0.0000;
        int current_iteration = 0;
        while (current_iteration != iterations) {
            clock_t start = clock();
            if (execute) {
#ifdef SWAN_SIMULATION
                char graph_name[100];
                sprintf(graph_name, "%s_%s_%d", library, kernel, SWAN_REG_TYPE);
                fake_neon_initializer(graph_name);
#endif
                kernel_func(config, input[idx], output[idx]);
#ifdef SWAN_SIMULATION
                fake_neon_finisher();
#endif
            }
            clock_t end = clock();

            clock_t clock_spent = end - start;
            time_spent += ((double)clock_spent) * ((double)1000000.0000) / ((double)CLOCKS_PER_SEC);

            current_iteration++;
            idx = (idx + 1) % count;

            // iterations > 0: energy evaluation mode (exit only from while condition)
            // time_spent must be more than 1 second
            // at least 10 iterations must be evaluated
            if ((iterations <= 0) && (time_spent >= 1000000.0000) && (current_iteration >= 10)) {
                break;
            }
#ifdef SWAN_SIMULATION
            break;
#endif
        }

        printf("Successfully finished run in");
#ifdef SWAN_SIMULATION
        printf("simulation mode!\n");
#else
        printf("experiment mode!\n");
#endif
        printf("iterations: %d (number of processing whole domain input)\n", current_iteration);
        printf("total_time: %lf usec (total execution time of all iterations)\n", time_spent);
        printf("iteration_time: %lf usec (execution time of one iterations)\n", time_spent / (double)current_iteration);
        printf("granularity: %d (number of individual kernel calls per iteration)\n", config->granularity);
        printf("kernel_time: %lf usec (execution time of one kernel call)\n", time_spent / (double)current_iteration / (double)config->granularity);
    }
}