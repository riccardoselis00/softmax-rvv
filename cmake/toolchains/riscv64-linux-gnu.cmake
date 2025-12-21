# Cross compile to RISC-V (Ubuntu toolchain riscv64-linux-gnu-*)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_C_COMPILER   riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)

# When cross-compiling, CMake must not try to run test executables on host
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Ensure CMake searches headers/libs in the target root
# (Ubuntu typically has /usr/riscv64-linux-gnu as the target sysroot)
set(CMAKE_FIND_ROOT_PATH /usr/riscv64-linux-gnu)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
