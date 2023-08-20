# Swan Benchmark Suite

A vector benchmark suite for mobile processors.

## Content

Swan contains scalar and vectorized (Arm Neon implementation) data-parallel kernels, taken from 12 frequently-used libraries of 4 real-world mobile applications:

1. [Chromium project](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/android_build_instructions.md) (web browser)
2. [Android](https://source.android.com/docs/setup/download/downloading) (operating system)
3. [WebRTC Project](https://webrtc.googlesource.com/src) (audio/video messaging)
4. [PDFium](https://pdfium.googlesource.com/pdfium/) (PDF rendering engine)

The following table shows the libraries and their usage across the evaluated mobile applications:

<div align="center">

| Library | Domain | Chromium | Android | WebRTC | PDFium |
| :------ | :----- | :------: | :-----: | :----: | :----: |
| [libjpeg-turbo](src/libraries/libjpeg/) | Image Processing | :heavy_check_mark: | :x: | :x: | :heavy_check_mark: |
| [libpng](src/libraries/libpng/) | Image Processing | :heavy_check_mark: | :x: | :x: | :heavy_check_mark: |
| [libwebp](src/libraries/libwebp/) | Image Processing | :heavy_check_mark: | :x: | :x: | :heavy_check_mark: |
| [Skia](src/libraries/skia/) | Graphics | :heavy_check_mark: | :heavy_check_mark: | :x: | :heavy_check_mark: |
| [WebAudio](src/libraries/webaudio/) | Audio Processing | :heavy_check_mark: | :x: | :heavy_check_mark: | :x: |
| [PFFFT](src/libraries/pffft/) | Audio Processing | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| [zlib](src/libraries/zlib/) | Data Compression | :heavy_check_mark: | :heavy_check_mark: | :x: | :heavy_check_mark: |
| [boringssl](src/libraries/boringssl/) | Cryptography | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| [Arm Optimized Routines](src/libraries/optroutines/) | String Utilities | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| [libopus](src/libraries/libopus/) | Audio Processing | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| [libvpx](src/libraries/libvpx/) | Video Processing | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :x: |
| [XNNPACK](src/libraries/xnnpack/) | Machine Learning | :heavy_check_mark: | :heavy_check_mark: | :x: | :x: |

</div>

### Directory structure

- [src/benchmark](/src/benchmark/): benchmark infrastructure to configure and launch kernels, generate input data, and compare output results.
- [src/libraries](/src/libraries/)`/[LIB]/[KER]`: scalar and vector implementation for `KER` kernel of `LIB` library.
- [src/fake_neon](/src/fake_neon/): Arm Neon intrinsic simulator.
- [src/scripts](/src/scripts/): Performance and power measurement scripts for Android Devices.

## Download

### Latest source code

Swan is compiled as an stand-alone tool without any dependencies.
Download the latest version of benchmark from this repository:

```bash
git clone git@github.com:arkhadem/Swan.git
```

### Input datasets

Kernels are equipped with *utility functions* to generate random inputs with the following charactrestics. 

- 720x1280 (HD) images for Image Processing, Graphics, and Video Processing libraries.
- 1 second of a standard audio stream with a 44.1 kHz sample rate for Audio Processing libraries.
- 128 KB data for Data Compression, Cryptography, and String Utility libraries.
- 156 layers of Convolutional Neural Networks for the MachineLearning library.

One can change the input sizes in the `src/benchmark/swan.hpp` header file.

## Setup

### Local Compilation

Compile Swan for local execution with:

```bash
make local -j<num_threads>
```

When compiling locally on a machine with any architecture rather than `armv8.2-a`, Swan switches to simulation mode (refer to [Arm Fake Neon Library](#arm-fake-neon-library)).

### Cross Compilation

Swan is tested with `android-ndk-r23c` for cross compilation for android devices with `armv8.2-a` architectures.
In addition, swan requires `fp16`, `crypto`, and `crc` extensions for XNNPACK, boringssl, and zlib libraries.

Download and extract [Android NDK](https://developer.android.com/ndk/downloads), and use the following instructions to setup the environmental variables.

```bash
export ANDROID_NDK_ROOT=/path/to/your/android-ndk-xxxx # replace xxx with NDK version (e.g., r23c)
export ANDROID_NDK_PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64
```

Cross-compile Swan with:

```bash
make phone -j<num_threads>
```

### Compilation Flags

Makefile accepts the following commandline arguments to configure the build.

- `CACHE=[WARM|COLD]`: warm-up or cold-down caches before execution. Default is `WARM`.
- `AUTOVEC=[FALSE|TRUE]`: using compiler auto-vectorization. Default is `FALSE`.
- `SIMMOD=[FALSE|TRUE]`: simulation mode for local execution. Default is `FALSE` for `armv8.2-a` machines and `TRUE` for any other architectures. Uses [Arm Fake Neon Library](#arm-fake-neon-library).
- `SIMREG=[128|256|512|1024]`: maximum supported width of [example kernels](#examples) developed with fake wide vector register implementations (affective only in simulation mode).

### Example Builds

```bash
# compiling locally in simulation mode with cold caches and compiler auto-vectorization.
# example kernels use up to 512-bit fake wide vector registers
make local CACHE=COLD AUTOVEC=TRUE SIMMOD=TRUE SIMREG=512

# cross-compiling for a phone with warm caches and no compiler auto-vectorization
make phone CACHE=WARM AUTOVEC=FALSE
```

## Running

Run the benchmark suite with:

```bash
./swan_[local|phone] -p [PLATFORM] -l [LIBRARY] -k [KERNEL]
```

- `PLATFORM`: scalar, neon, or compare (comparing scalar and neon output results)
- `LIBRARY`: name of the target library
- `KERNEL`: name of the target kernel

Use `--help` for more options or `--list` to get a list of supported libraries and kernels on the target machine architecture.

### Performance and Power Measurement of Android Devices

Having built Swan for phone, you can use the [profiler script](/scripts/profiler.py) to dump profile results in a CSV file:

```bash
./scripts/profiler.py --measurement [power|performance] --output [profile.csv]
```

Profiler requires [Android Debug Bridge (adb)](https://developer.android.com/tools/releases/platform-tools) installed locally.
Use `--help` for more options.

**Recommendation:**

- Configure core masks (CPU affinities) in [scripts/mask.py](/scripts/mask.py) based on your Android Device CPU configuration and use `--core [core_name]` to pin Swan process to a specific core. 
- Use [adb wireless debugging](https://developer.android.com/tools/adb), specifically for power measurements.

## Arm Fake Neon Library

Swan is equipped with a library of fake Arm Neon intrinsics that is used in **simulation mode** for multiple purposes:

- Fake Neon library extends 64 (*D*) and 128-bit (*Q*) vector registers of Arm Neon to 256 (*Double-Q* or *DQ*), 512 (*Quad-Q* or *QQ*), and 1024-bit (*Octa-Q* or *OQ*) registers. This library implements all intrinsics for all data widths, which could be used to study the scalability of mobile data-parallel applications with wider registers.
- Using this library, executing Swan is not limited to `armv8.2-a` architecture and can be simulated on any architecture such as `X86-64`.

### Usage

Simply include `fake_neon.hpp` (instead of `arm_neon.h`) and compile `src/fake_neon/*.cpp` along with your vectorized kernel source code.

Before and after calling a vectorized kernel, inject Fake Neon initializer and finisher APIs:

```cpp
fake_neon_initializer("kernel_name");
vectorized_kernel();
fake_neon_finisher();
```

### Outputs

Executing a kernel in simulation mode dumps vector instruction Data-Flow Graph (DFG) in a file named `[kernel_name]_dfg.txt`.
DFG graph enables studying different charactrestics of vectorized data-parallel kernels such as Instruction-Level Parallelism (ILP).

### Examples

The following kernels are extended with wide fake Neon implementations.
Please refer to their Neon source code to see the examples.

<div align="center">

| Library | Kernel |
| :------ | :----- |
| [libjpeg-turbo](src/libraries/libjpeg/) | [RGB2YCbCr](src/libraries/libjpeg/rgb_to_ycbcr/neon.cpp) |
| [libwebp](src/libraries/libwebp/) | [TM Prediction](src/libraries/libwebp/tm_prediction/neon.cpp) |
| [Skia](src/libraries/skia/) | [Convolve Horizontally](src/libraries/skia/convolve_horizontally/neon.cpp) |
| [Webaudio](src/libraries/webaudio/) | [Audible](src/libraries/webaudio/is_audible/neon.cpp) |
| [zlib](src/libraries/zlib/) | [Adler-32](src/libraries/zlib/adler32/neon.cpp) |
| [libopus](src/libraries/libopus/) | [Pitch Correlation](src/libraries/libopus/pitch_xcorr/neon.cpp) |
| [libvpx](src/libraries/libvpx/) | [SAD](src/libraries/libvpx/sad/neon.cpp) |
| [XNNPACK](src/libraries/xnnpack/) | [FP32 GEMM](src/libraries/xnnpack/gemm/neon.cpp) |

</div>

### Limitations

The current version of Fake Neon library does not support:
- `fp16` extension: FP16 implementations of GEMM and SpMM kernels of XNNPACK library.
- `crypto` extension: all kernels of boringssl library.
- `crc` extension: CRC32 kernel of zlib library.
- assembly code: all kernels of Arm Optimized Routines library.

## Citation

If you use Swan or find Swan useful, please cite this paper:

> Alireza Khadem, Daichi Fujiki, Nishil Talati, Scott Mahlke, and Reetuparna Das.
> *Vector-Processing for Mobile Devices: Benchmark and Analysis*,
> In 2023 IEEE International Symposium on Workload Characterization (IISWC)

```
@inproceedings{swan,
    title={Vector-Processing for Mobile Devices: Benchmark and Analysis},
    author={Khadem, Alireza and Fujiki, Daichi and Talati, Nishil and Mahlke, Scott and Das, Reetuparna},
    booktitle={2023 IEEE International Symposium on Workload Characterization (IISWC)},
    year={2023}
}
```

## Issues and bug reporting

Swan is under active development. We appreciate any feedback and suggestions from the community.
Feel free to raise an issue or submit a pull request on Github.
For assistance in using Swan, please contact: Alireza Khadem (arkhadem@umich.edu)

## Licensing

Each kernel is individually licensed according to the library it is extracted from.
Swan benchmarking infrastructure and Fake Neon library is available under a [MIT license](/LICENSE).

## Acknowledgement

This work was supported in part by the NSF under CAREER-1652294 and NSF-1908601 awards and the Applications Driving Architectures (ADA) Research Center, a JUMP Center co-sponsored by SRC and DARPA.