cmake_minimum_required (VERSION 3.1.3) 
project (wave2d_package)

set (CMAKE_CXX_STANDARD 11)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (BUILD_SHARED_LIBS true)

option (USE_CUDA "Enable CUDA" OFF)
option (USE_QT "Enable Qt" OFF)
option (USE_OpenMP "Enable OpenMP" OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
