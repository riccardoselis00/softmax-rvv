# softmax-rvv/benches/CMakeLists.txt

set(ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(SOFT_BUILD_TESTS   "Build correctness tests" ON)
option(SOFT_BUILD_BENCHES "Build performance benches" ON)

# You already pass these from CLI
set(SOFT_MARCH "" CACHE STRING "Value for -march=...")
set(SOFT_MABI  "" CACHE STRING "Value for -mabi=... (optional)")

# --------------------------------------------------
# ISA flags
# --------------------------------------------------
function(soft_apply_isa_flags tgt)
  if(SOFT_MARCH)
    target_compile_options(${tgt} PUBLIC "-march=${SOFT_MARCH}")
    target_link_options(${tgt}    PUBLIC "-march=${SOFT_MARCH}")
  endif()
  if(SOFT_MABI)
    target_compile_options(${tgt} PUBLIC "-mabi=${SOFT_MABI}")
    target_link_options(${tgt}    PUBLIC "-mabi=${SOFT_MABI}")
  endif()
endfunction()

function(soft_maybe_static tgt)
  if(SOFT_BENCH_STATIC AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(${tgt} PRIVATE -static)
  endif()
endfunction()

# --------------------------------------------------
# gem5 m5ops (ROI support)
# --------------------------------------------------
option(SOFT_USE_M5 "Link gem5 m5ops/libm5 to enable ROI stats" OFF)

set(GEM5_ROOT "" CACHE PATH "Path to gem5 source root")

if(SOFT_USE_M5 AND NOT GEM5_ROOT)
  if(EXISTS "$ENV{HOME}/Desktop/gem5/include")
    set(GEM5_ROOT "$ENV{HOME}/Desktop/gem5" CACHE PATH "Path to gem5 source root" FORCE)
  endif()
endif()

set(SOFT_M5_INCLUDE_DIR "${GEM5_ROOT}/include" CACHE PATH "gem5 include dir")
set(SOFT_M5_LIB "${GEM5_ROOT}/util/m5/build/riscv/out/libm5.a"
    CACHE FILEPATH "Path to RISC-V libm5.a")

function(soft_link_m5 tgt)
  if(NOT SOFT_USE_M5)
    target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=0)
    return()
  endif()

  # Only for RISC-V builds
  if(NOT (CMAKE_CROSSCOMPILING OR CMAKE_SYSTEM_PROCESSOR MATCHES "riscv"))
    message(WARNING
      "SOFT_USE_M5=ON but this looks like a native build. "
      "Skipping libm5 for target '${tgt}'.")
    target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=0)
    return()
  endif()

  if(NOT EXISTS "${SOFT_M5_INCLUDE_DIR}/gem5/m5ops.h")
    message(FATAL_ERROR "Missing m5ops.h: ${SOFT_M5_INCLUDE_DIR}/gem5/m5ops.h")
  endif()

  if(NOT EXISTS "${SOFT_M5_LIB}")
    message(FATAL_ERROR "Missing libm5.a: ${SOFT_M5_LIB}")
  endif()

  target_include_directories(${tgt} PRIVATE "${SOFT_M5_INCLUDE_DIR}")
  target_link_libraries(${tgt}      PRIVATE "${SOFT_M5_LIB}")
  target_compile_definitions(${tgt} PRIVATE SOFT_USE_M5=1)
endfunction()

# --------------------------------------------------
# common library
# --------------------------------------------------
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

# --------------------------------------------------
# kernels
# --------------------------------------------------
add_library(soft_kernels STATIC
  ${ROOT}/kernels/poly_horner.c
)

target_include_directories(soft_kernels PUBLIC
  ${ROOT}/kernels
  ${ROOT}/common
)

target_link_libraries(soft_kernels PUBLIC soft_common)
soft_apply_isa_flags(soft_kernels)

# --------------------------------------------------
# tests
# --------------------------------------------------
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

  if(NOT CMAKE_CROSSCOMPILING)
    add_test(NAME test_macacc      COMMAND test_macacc)
    add_test(NAME test_cdiv        COMMAND test_cdiv)
    add_test(NAME test_poly_horner COMMAND test_poly_horner)
  endif()
endif()

# --------------------------------------------------
# benches
# --------------------------------------------------
if(SOFT_BUILD_BENCHES)

  # Poly-Horner
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

  # PDIV8
  add_executable(bench_pdiv8_ref ${ROOT}/benches/bench_pdiv8_ref.c)
  target_link_libraries(bench_pdiv8_ref PRIVATE soft_common)
  target_include_directories(bench_pdiv8_ref PRIVATE ${ROOT}/common)
  soft_apply_isa_flags(bench_pdiv8_ref)
  soft_maybe_static(bench_pdiv8_ref)
  soft_link_m5(bench_pdiv8_ref)

  add_executable(bench_pdiv8_custom ${ROOT}/benches/bench_pdiv8_custom.c)
  target_link_libraries(bench_pdiv8_custom PRIVATE soft_common)
  target_include_directories(bench_pdiv8_custom PRIVATE ${ROOT}/common)
  target_compile_definitions(bench_pdiv8_custom PRIVATE SOFT_HAVE_CUSTOM_OPS=1)
  soft_apply_isa_flags(bench_pdiv8_custom)
  soft_maybe_static(bench_pdiv8_custom)
  soft_link_m5(bench_pdiv8_custom)

  add_executable(bench_softmax_custom ${ROOT}/benches/bench_softmax_custom.c)
  target_link_libraries(bench_softmax_custom PRIVATE soft_common)
  target_include_directories(bench_softmax_custom PRIVATE ${ROOT}/common)
  target_compile_definitions(bench_softmax_custom PRIVATE SOFT_HAVE_CUSTOM_OPS=1)
  soft_apply_isa_flags(bench_softmax_custom)
  soft_maybe_static(bench_softmax_custom)
  soft_link_m5(bench_softmax_custom)

  add_executable(bench_softmax_ref ${ROOT}/benches/bench_softmax_ref.c)
  target_link_libraries(bench_softmax_ref PRIVATE soft_common)
  target_include_directories(bench_softmax_ref PRIVATE ${ROOT}/common)
  soft_apply_isa_flags(bench_softmax_ref)
  soft_maybe_static(bench_softmax_ref)
  soft_link_m5(bench_softmax_ref)


endif()
