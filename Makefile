red:=$(shell tput setaf 1)
green:=$(shell tput setaf 2)
reset:=$(shell tput sgr0)

SRC_DIR = src
OBJ_DIR = obj

BENCH_DIR = $(SRC_DIR)/benchmark
LOCAL_OBJ_DIR = $(OBJ_DIR)/local_obj
PHONE_OBJ_DIR = $(OBJ_DIR)/phone_obj
RISCV_OBJ_DIR = $(OBJ_DIR)/riscv_obj
LIB_SRC_DIR = $(SRC_DIR)/libraries
FAKE_NEON_DIR = $(SRC_DIR)/fake_neon
UNAME_P := $(shell uname -p)

INIT_KERNELS := $(wildcard $(LIB_SRC_DIR)/*/*/utility.cpp) $(BENCH_DIR)/utility.cpp
SCALAR_KERNELS := $(wildcard $(LIB_SRC_DIR)/*/*/scalar.cpp) $(wildcard $(LIB_SRC_DIR)/*/scalar.cpp) $(BENCH_DIR)/scalar.cpp
NEON_KERNELS := $(wildcard $(LIB_SRC_DIR)/*/*/neon.cpp) $(wildcard $(LIB_SRC_DIR)/*/neon.cpp) $(wildcard $(LIB_SRC_DIR)/*/*/neon_assembly.S) $(BENCH_DIR)/neon.cpp
LIB_KERNELS := $(LIB_SRC_DIR)/pffft/pffft.cpp
RUN_KERNELS := $(BENCH_DIR)/runner.cpp
ALL_KERNELS := $(INIT_KERNELS) $(SCALAR_KERNELS) $(NEON_KERNELS) $(LIB_KERNELS) $(RUN_KERNELS)
FAKE_NEON_KERNELS := $(wildcard $(FAKE_NEON_DIR)/*.cpp)
LOCAL_TMP_OBJS = $(ALL_KERNELS:%.cpp=$(LOCAL_OBJ_DIR)/%.o)
PHONE_TMP_OBJS = $(ALL_KERNELS:%.cpp=$(PHONE_OBJ_DIR)/%.o)
RISCV_TMP_OBJS = $(ALL_KERNELS:%.cpp=$(RISCV_OBJ_DIR)/%.o)
LOCAL_OBJS = $(LOCAL_TMP_OBJS:%.S=$(LOCAL_OBJ_DIR)/%.o)
PHONE_OBJS = $(PHONE_TMP_OBJS:%.S=$(PHONE_OBJ_DIR)/%.o)
RISCV_OBJS = $(RISCV_TMP_OBJS:%.S=$(RISCV_OBJ_DIR)/%.o)

INC_PARAMS := $(foreach d, $(dir $(wildcard $(BENCH_DIR)/*.hpp)), -I$d)
INC_PARAMS += $(foreach d, $(dir $(wildcard $(LIB_SRC_DIR)/*/*.hpp)), -I$d)
INC_PARAMS += $(foreach d, $(dir $(wildcard $(LIB_SRC_DIR)/*/*/*.hpp)), -I$d)

CFLAGS = -O3 -fno-builtin
# Information on -fno-builtin:
# When compiling without -fno-builtin, the compiler can replace calls to certain standard C library functions
# with inline code or with calls to other library functions. Subsequently, your re-implementations of the
# standard C library functions might not be used, and might be removed by the linker.

# Default compilation: no-auto-vector, warm-cache, execution-mode
CACHE = WARM
AUTOVEC = FALSE
DEBUG = FALSE
SIMMOD = FALSE
SIMREG = 128

ifeq ($(CACHE),WARM)
    $(info Cache Status -> Warm)
    CFLAGS += -DCACHE_STATUS=CACHE_STATUS_WARM
else
    $(info Cache Status -> Cold)
    CFLAGS += -DCACHE_STATUS=CACHE_STATUS_COLD
endif

ifeq ($(AUTOVEC),FALSE)
    $(info Auto Vectorization -> False)
    CFLAGS += -fno-builtin -fno-tree-vectorize -fno-vectorize -fno-slp-vectorize
else
    $(info Auto Vectorization -> True)
endif

ifeq ($(DEBUG),TRUE)
    $(info Debug Mode -> True)
    CFLAGS += -g -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -fsave-optimization-record
else
    $(info Debug Mode -> False)
endif

BASE_CFLAGS = $(CFLAGS) -Wall -Wno-unused-variable -Wno-unused-command-line-argument -DLOG_USE_COLOR -lm -std=c++17
LOCAL_CFLAGS := $(BASE_CFLAGS)
PHONE_CFLAGS := $(INC_PARAMS) $(BASE_CFLAGS) --target=aarch64-none-linux-android23 --sysroot=$(ANDROID_NDK_PATH)/sysroot -march=armv8.2-a+fp16+crypto+crc -DNEON
RISCV_CFLAGS := $(INC_PARAMS) $(BASE_CFLAGS)

ifeq ($(UNAME_P),x86_64)
    SIMMOD = TRUE
endif

RISCV_CFLAGS += -DSWAN_SIMULATION
ifeq ($(SIMREG),128)
    RISCV_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_128_TYPE
else ifeq ($(SIMREG),256)
    RISCV_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_256_TYPE
else ifeq ($(SIMREG),512)
    RISCV_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_512_TYPE
else ifeq ($(SIMREG),1024)
    RISCV_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_1024_TYPE
endif
RISCV_OBJS += $(FAKE_NEON_KERNELS:%.cpp=$(RISCV_OBJ_DIR)/%.o)
RISCV_OBJS := $(filter-out $(RISCV_OBJ_DIR)/src/libraries/boringssl/%,$(RISCV_OBJS))
RISCV_OBJS := $(filter-out $(RISCV_OBJ_DIR)/src/libraries/zlib/crc32/%,$(RISCV_OBJS))
RISCV_OBJS := $(filter-out $(RISCV_OBJ_DIR)/src/libraries/optroutines/%,$(RISCV_OBJS))
RISCV_OBJS := $(filter-out $(RISCV_OBJ_DIR)/src/libraries/pffft/%,$(RISCV_OBJS))
RISCV_CFLAGS += $(foreach d, $(dir $(wildcard $(FAKE_NEON_DIR)/*.hpp)), -I$d)

ifeq ($(SIMMOD),FALSE)
    $(info Simulation Mode -> OFF)
    LOCAL_CFLAGS += -march=armv8.2-a+fp16+crypto+crc -DNEON
else
    $(info Simulation Mode -> ON)
    LOCAL_CFLAGS += -DSWAN_SIMULATION
    ifeq ($(SIMREG),128)
        LOCAL_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_128_TYPE
    else ifeq ($(SIMREG),256)
        LOCAL_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_256_TYPE
    else ifeq ($(SIMREG),512)
        LOCAL_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_512_TYPE
    else ifeq ($(SIMREG),1024)
        LOCAL_CFLAGS += -DSWAN_SIM_REG_TYPE=SWAN_REG_1024_TYPE
    endif
    LOCAL_OBJS += $(FAKE_NEON_KERNELS:%.cpp=$(LOCAL_OBJ_DIR)/%.o)
    LOCAL_OBJS := $(filter-out $(LOCAL_OBJ_DIR)/src/libraries/boringssl/%,$(LOCAL_OBJS))
    LOCAL_OBJS := $(filter-out $(LOCAL_OBJ_DIR)/src/libraries/zlib/crc32/%,$(LOCAL_OBJS))
    LOCAL_OBJS := $(filter-out $(LOCAL_OBJ_DIR)/src/libraries/optroutines/%,$(LOCAL_OBJS))
    LOCAL_OBJS := $(filter-out $(LOCAL_OBJ_DIR)/src/libraries/pffft/%,$(LOCAL_OBJS))
    INC_PARAMS += $(foreach d, $(dir $(wildcard $(FAKE_NEON_DIR)/*.hpp)), -I$d)
endif
LOCAL_CFLAGS := $(INC_PARAMS) $(LOCAL_CFLAGS)

LOCAL_CPP = clang++
PHONE_CPP = $(ANDROID_NDK_PATH)/bin/clang++
RISCV_CPP = riscv64-unknown-elf-g++

$(LOCAL_OBJ_DIR)/%.o: %.cpp
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR LOCAL)
	@mkdir -p '$(@D)'
	@$(LOCAL_CPP) -c $(LOCAL_CFLAGS) $< -o $@

$(LOCAL_OBJ_DIR)/%.o: %.S
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR LOCAL)
	@mkdir -p '$(@D)'
	@$(LOCAL_CPP) -c $(LOCAL_CFLAGS) $< -o $@

$(PHONE_OBJ_DIR)/%.o: %.cpp
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR PHONE)
	@mkdir -p '$(@D)'
	@$(PHONE_CPP) -c $(PHONE_CFLAGS) $< -o $@

$(PHONE_OBJ_DIR)/%.o: %.S
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR PHONE)
	@mkdir -p '$(@D)'
	@$(PHONE_CPP) -c $(PHONE_CFLAGS) $< -o $@

$(RISCV_OBJ_DIR)/%.o: %.cpp
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR RISCV)
	@mkdir -p '$(@D)'
	@$(RISCV_CPP) -c $(RISCV_CFLAGS) $< -o $@

$(RISCV_OBJ_DIR)/%.o: %.S
	$(info COMPILING SOURCE $(red)$<$(reset) INTO OBJECT $(green)$@$(reset) FOR RISCV)
	@mkdir -p '$(@D)'
	@$(RISCV_CPP) -c $(RISCV_CFLAGS) $< -o $@

all: local

local: $(LOCAL_OBJS) $(BENCH_DIR)/main.cpp
	@$(LOCAL_CPP) $(LOCAL_CFLAGS) -o swan_$@ $?

phone: $(PHONE_OBJS) $(BENCH_DIR)/main.cpp
	@$(PHONE_CPP) $(PHONE_CFLAGS) -o swan_$@ $?

riscv: $(RISCV_OBJS) $(BENCH_DIR)/main.cpp
	@$(RISCV_CPP) $(RISCV_CFLAGS) -o swan_$@ $?

push:
	adb push swan_phone $(PHONE_SWAN_DIR)/

.PHONY: clean
clean:
	@rm -rf $(LOCAL_OBJ_DIR) $(PHONE_OBJ_DIR)
	@rm -rf swan_local
	@rm -rf swan_phone
	@rm -rf swan_riscv
