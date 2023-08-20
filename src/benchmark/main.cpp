#include "swan.hpp"
#include <cstddef>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>

#include "argparse.hpp"

long *pollute_cache(size_t size) {
    const size_t bigger_than_cachesize = size / sizeof(long);
    long *p = (long *)malloc(size);
    for (int i = 0; i < bigger_than_cachesize; i++) {
        p[i] = i;
    }
    return p;
}

int main(int argc, char *argv[]) {

#ifndef SWAN_SIMULATION
    argparse::ArgumentParser program("swan_phone");
#else
    argparse::ArgumentParser program("swan_local");
#endif

    program.add_argument("-p", "--platform")
        .help("\"scalar\", \"neon\", or \"compare\" (comparing scalar and neon output results)")
        .metavar("PLATFORM");

    program.add_argument("-l", "--library")
        .help("Name of the library. Use --list for a list of libraries and kernels")
        .metavar("LIBRARY");

    program.add_argument("-k", "--kernel")
        .help("Name of the kernel. Use --list for a list of libraries and kernels")
        .metavar("KERNEL");

    program.add_argument("-s", "--list")
        .help("Show supported libraries and kernels based on the machine configuration")
        .default_value(false)
        .implicit_value(true);

#ifndef SWAN_SIMULATION
    program.add_argument("-i", "--iterations")
        .help("Specific number of iterations that the experiment executes. If not specified, experiment runs for one second")
        .default_value(-1)
        .scan<'i', int>()
        .metavar("ITERATIONS");

    program.add_argument("-e", "--execute")
        .help("When specifying this option, kernels will not be invoked (used for energy measurements)")
        .default_value(true)
        .implicit_value(true);
#endif

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    bool list = (program["--list"] == true);

    std::string library, kernel, platform_str;
    platform_t platform_type = platform_t::NEON_PLAT;

    if (list == false) {
        if ((program.present("--platform") == std::nullopt) ||
            (program.present("--library") == std::nullopt) ||
            (program.present("--kernel") == std::nullopt)) {
            printf("Error: platform, library, and kernel must be provided!\n");
            printf("%s\n", program.help().str().c_str());
            exit(-1);
        }
        platform_str = program.get<std::string>("--platform");
        library = program.get<std::string>("--library").c_str();
        kernel = program.get<std::string>("--kernel").c_str();
        if (platform_str.compare("scalar") == 0) {
            platform_type = platform_t::SCALAR_PLAT;
        } else if (platform_str.compare("neon") == 0) {
            platform_type = platform_t::NEON_PLAT;
        } else if (platform_str.compare("compare") == 0) {
            platform_type = platform_t::MAX_PLAT;
        } else {
            printf("Error: platform must be either \"scalar\" or \"neon\" or \"compare\".\n");
            exit(-1);
        }
    }
#ifndef SWAN_SIMULATION
    int iterations = program.get<int>("--iterations");
    bool execute = (program["--execute"] == true);
#else
    int iterations = -1;
    bool execute = true;
#endif

    benchmark_runner(platform_type, library.c_str(), kernel.c_str(), iterations, execute, list);
    return 0;
}