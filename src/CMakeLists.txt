cmake_minimum_required (VERSION 3.1.3) 
project (src)

set(WAVE2D_REAL_TYPE float CACHE STRING "Real type for wave2d (float or double)")

add_definitions(-DWAVE2D_REAL_TYPE=${WAVE2D_REAL_TYPE})

add_subdirectory(wave2d)
add_subdirectory(animation)
add_subdirectory(solver_ref)
add_subdirectory(solver_omp)
add_subdirectory(util)

if (USE_CUDA)
    add_subdirectory(solver_cuda)
    add_definitions(-DWAVES2D_USE_CUDA)
endif()
