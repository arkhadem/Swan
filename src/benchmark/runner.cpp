#include "swan.hpp"

#include "runner.hpp"
#include "utility.hpp"
#include <stddef.h>
#include <stdio.h>

#ifdef SWAN_SIMULATION
#include <fake_neon.h>
#endif

std::map<std::string, std::map<std::string, std::map<platform_e, kernel_func>>> kernel_functions;

void dummy_finisher() {
    kernel_functions["dummy_lib"]["dymmy_kernel"][platform_e::MAX_PLAT] = NULL;
}

void benchmark_runner(platform_t platform, char *library, char *kernel, int rounds, bool execute) {
    register_utilities();
    register_scalar_kernels();
    register_neon_kernels();
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
    std::printf("Cache polluted (%ld) | Count (%d)\n", tmp[rand() % CACHE_SIZE], count);
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
        int iterations = 0;
        // in us
        double time_spent = 0.0000;
        int current_round = 0;
        while (current_round != rounds) {

            iterations++;

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
#ifdef DYNAMORIO
                dummy_finisher();
#endif
            }
            clock_t end = clock();

            clock_t clock_spent = end - start;
            time_spent += ((double)clock_spent) * ((double)1000000.0000) / ((double)CLOCKS_PER_SEC);

            current_round++;
            idx = (idx + 1) % count;

            // rounds > 0: energy evaluation mode (exit only from while condition)
            // time_spent must be more than 1 second
            // at least 10 rounds must be evaluated
            if ((rounds <= 0) && (time_spent >= 1000000.0000) && (current_round >= 10)) {
                break;
            }
#ifdef SWAN_SIMULATION
            break;
#endif
        }

#ifndef SWAN_SIMULATION
        printf("Finished in"
               " Total(%lf usec, %d iterations, %d granularity)"
               " Raw(%lf usec, %d granularity)"
               " Individual(%lf usec)\n",
               time_spent, iterations, config->granularity,
               time_spent / (double)iterations, config->granularity,
               time_spent / (double)iterations / (double)config->granularity);
#endif
    }
}