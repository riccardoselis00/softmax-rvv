# softmax-rvv/benches/CMakeLists.txt

set(ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(SOFT_BUILD_TESTS   "Build correctness tests" ON)
option(SOFT_BUILD_BENCHES "Build performance benches" ON)

# You already pass SOFT_MARCH, SOFT_MABI, SOFT_HAVE_CUSTOM_OPS, SOFT_BENCH_STATIC from command line
set(SOFT_MARCH "" CACHE STRING "Value for -march=...")
set(SOFT_MABI  "" CACHE STRING "Value for -mabi=... (optional)")

function(soft_apply_isa_flags tgt)
  if(SOFT_MARCH)
    target_compile_options(${tgt} PUBLIC "-march=${SOFT_MARCH}")
    target_link_options(${tgt} PUBLIC "-march=${SOFT_MARCH}")
  endif()
  if(SOFT_MABI)
    target_compile_options(${tgt} PUBLIC "-mabi=${SOFT_MABI}")
    target_link_options(${tgt} PUBLIC "-mabi=${SOFT_MABI}")
  endif()
endfunction()

function(soft_maybe_static tgt)
  if(SOFT_BENCH_STATIC AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(${tgt} PRIVATE -static)
  endif()
endfunction()

# ---------------- gem5 m5ops (optional) ----------------
option(SOFT_USE_M5 "Link gem5 m5ops/libm5 to enable m5_reset_stats/m5_dump_stats" OFF)

# Path to gem5 source root (must contain include/ and util/m5/)
set(GEM5_ROOT "" CACHE PATH "Path to gem5 source root (contains include/ and util/m5/)")

# Convenience default for your local setup; still overrideable from CLI
if(SOFT_USE_M5 AND NOT GEM5_ROOT)
  if(EXISTS "$ENV{HOME}/Desktop/gem5/include")
    set(GEM5_ROOT "$ENV{HOME}/Desktop/gem5" CACHE PATH "Path to gem5 source root" FORCE)
  endif()
endif()

# Where <gem5/m5ops.h> lives
set(SOFT_M5_INCLUDE_DIR "${GEM5_ROOT}/include" CACHE PATH "gem5 include dir (contains gem5/m5ops.h)")

# Your already-built RISC-V libm5.a
set(SOFT_M5_LIB "${GEM5_ROOT}/util/m5/build/riscv/out/libm5.a" CACHE FILEPATH "Path to libm5.a (RISC-V)")

function(soft_link_m5 tgt)
  if(NOT SOFT_USE_M5)
    target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=0)
    return()
  endif()

  # Only link libm5 when the *target* is RISC-V.
  # NOTE: This requires configuring CMake with a RISC-V toolchain file.
  if(NOT (CMAKE_CROSSCOMPILING OR CMAKE_SYSTEM_PROCESSOR MATCHES "riscv"))
    message(WARNING
      "SOFT_USE_M5=ON but this looks like a native (non-RISC-V) build "
      "(CMAKE_SYSTEM_PROCESSOR='${CMAKE_SYSTEM_PROCESSOR}', CMAKE_CROSSCOMPILING='${CMAKE_CROSSCOMPILING}'). "
      "Skipping libm5 link for target '${tgt}'. "
      "Reconfigure with a RISC-V toolchain file to enable m5.")
    target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=0)
    return()
  endif()

  if(NOT EXISTS "${SOFT_M5_INCLUDE_DIR}/gem5/m5ops.h")
    message(FATAL_ERROR "m5ops header not found: ${SOFT_M5_INCLUDE_DIR}/gem5/m5ops.h")
  endif()
  if(NOT EXISTS "${SOFT_M5_LIB}")
    message(FATAL_ERROR "libm5.a not found: ${SOFT_M5_LIB}")
  endif()

  target_include_directories(${tgt} PRIVATE "${SOFT_M5_INCLUDE_DIR}")
  target_link_libraries(${tgt} PRIVATE "${SOFT_M5_LIB}")
  target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=1)
endfunction()

# ---------------- common ----------------
add_library(soft_common STATIC
  ${ROOT}/common/bench.c
  ${ROOT}/common/ops_ref.c
)

target_include_directories(soft_common PUBLIC
  ${ROOT}/common
)

target_compile_options(soft_common PRIVATE -O3 -Wall -Wextra -Wpedantic)
soft_apply_isa_flags(soft_common)

if(SOFT_HAVE_CUSTOM_OPS)
  target_sources(soft_common PRIVATE ${ROOT}/common/ops_custom.c)
  target_compile_definitions(soft_common PUBLIC SOFT_HAVE_CUSTOM_OPS=1)
else()
  target_compile_definitions(soft_common PUBLIC SOFT_HAVE_CUSTOM_OPS=0)
endif()

# ---------------- kernels ----------------
add_library(soft_kernels STATIC
  ${ROOT}/kernels/poly_horner.c
)

target_include_directories(soft_kernels PUBLIC
  ${ROOT}/kernels
  ${ROOT}/common
)

target_link_libraries(soft_kernels PUBLIC soft_common)
soft_apply_isa_flags(soft_kernels)

# ---------------- tests ----------------
function(add_soft_test name src)
  add_executable(${name} ${src})
  target_link_libraries(${name} PRIVATE soft_kernels soft_common)
  target_include_directories(${name} PRIVATE ${ROOT}/kernels ${ROOT}/common)
  soft_apply_isa_flags(${name})
  soft_maybe_static(${name})
  soft_link_m5(${name})
endfunction()

if(SOFT_BUILD_TESTS)
  enable_testing()

  add_soft_test(test_macacc      ${ROOT}/kernels/tests/test_macacc.c)
  add_soft_test(test_cdiv        ${ROOT}/kernels/tests/test_cdiv.c)
  add_soft_test(test_poly_horner ${ROOT}/kernels/tests/test_poly_horner.c)

  # NOTE: when cross-compiling you cannot run ctest on host.
  if(NOT CMAKE_CROSSCOMPILING)
    add_test(NAME test_macacc      COMMAND test_macacc)
    add_test(NAME test_cdiv        COMMAND test_cdiv)
    add_test(NAME test_poly_horner COMMAND test_poly_horner)
  endif()
endif()

# ---------------- benches ----------------
if(SOFT_BUILD_BENCHES)
  add_executable(bench_poly_horner_ref ${ROOT}/benches/bench_poly_horner_ref.c)
  target_link_libraries(bench_poly_horner_ref PRIVATE soft_kernels soft_common)
  target_include_directories(bench_poly_horner_ref PRIVATE ${ROOT}/kernels ${ROOT}/common)
  soft_apply_isa_flags(bench_poly_horner_ref)
  soft_maybe_static(bench_poly_horner_ref)
  soft_link_m5(bench_poly_horner_ref)

  add_executable(bench_poly_horner_custom ${ROOT}/benches/bench_poly_horner_custom.c)
  target_link_libraries(bench_poly_horner_custom PRIVATE soft_kernels soft_common)
  target_include_directories(bench_poly_horner_custom PRIVATE ${ROOT}/kernels ${ROOT}/common)
  soft_apply_isa_flags(bench_poly_horner_custom)
  soft_maybe_static(bench_poly_horner_custom)
  soft_link_m5(bench_poly_horner_custom)
endif()
