cmake_minimum_required(VERSION 3.10)
project(AdamAnalysis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable optimizations for Release builds
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(src)

# Core optimizer and function sources
set(CORE_SOURCES
    src/Adam.cpp
    src/SGDMomentum.cpp
    src/AMSGrad.cpp
    src/functions.cpp
)

# Main experiment runner (all experiments in one binary)
add_executable(AdamAnalysis src/main.cpp ${CORE_SOURCES})

# Individual experiment executables (kept for modularity)
add_executable(exp_rosenbrock experiments/rosenbrock/exp_rosenbrock.cpp ${CORE_SOURCES})
add_executable(exp_reddi experiments/reddi/exp_reddi.cpp ${CORE_SOURCES})
add_executable(exp_rastrigin experiments/rastrigin/exp_rastrigin.cpp ${CORE_SOURCES})

enable_testing()
add_executable(optimizer_tests tests/optimizer_tests.cpp ${CORE_SOURCES})
add_test(NAME optimizer_tests COMMAND optimizer_tests)
