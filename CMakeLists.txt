cmake_minimum_required(VERSION 3.20)
project(softmax_rvv_benches LANGUAGES C)

# ---------------- Options ----------------
option(SOFT_BUILD_TESTS       "Build correctness tests" ON)
option(SOFT_BUILD_BENCHES     "Build performance benches" ON)
option(SOFT_HAVE_CUSTOM_OPS   "Compile custom-op backend (.insn)" ON)
option(SOFT_ENABLE_RVV        "Add -march/-mabi flags (if appropriate)" OFF)
option(SOFT_BENCH_STATIC      "Try to link executables statically" OFF)

include(CTest)   # defines BUILD_TESTING and calls enable_testing() when ON
enable_testing() # safe even if you don’t add tests

# ---------------- Defaults ----------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(benches)
