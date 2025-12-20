# softmax-rvv/benches/CMakeLists.txt

set(ROOT ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(SOFT_BUILD_TESTS   "Build correctness tests" ON)
option(SOFT_BUILD_BENCHES "Build performance benches" ON)

# You already pass SOFT_MARCH, SOFT_HAVE_CUSTOM_OPS, SOFT_BENCH_STATIC from command line
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
  add_executable(bench_poly_horner_ref    ${ROOT}/benches/bench_poly_horner_ref.c)
  target_link_libraries(bench_poly_horner_ref PRIVATE soft_kernels soft_common)
  target_include_directories(bench_poly_horner_ref PRIVATE ${ROOT}/kernels ${ROOT}/common)
  soft_apply_isa_flags(bench_poly_horner_ref)
  soft_maybe_static(bench_poly_horner_ref)

  add_executable(bench_poly_horner_custom ${ROOT}/benches/bench_poly_horner_custom.c)
  target_link_libraries(bench_poly_horner_custom PRIVATE soft_kernels soft_common)
  target_include_directories(bench_poly_horner_custom PRIVATE ${ROOT}/kernels ${ROOT}/common)
  soft_apply_isa_flags(bench_poly_horner_custom)
  soft_maybe_static(bench_poly_horner_custom)
endif()
