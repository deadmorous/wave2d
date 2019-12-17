cmake_minimum_required (VERSION 3.1.3) 

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ProjectId})

if (USE_QT)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
    add_definitions(-DWAVE2D_USE_QT)
endif()


aux_source_directory(. SOURCE_FILES)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} animation solver_ref)
target_link_libraries(${PROJECT_NAME} animation solver_ref)

if (USE_CUDA)
    add_dependencies(${PROJECT_NAME} animation solver_cuda)
    target_link_libraries(${PROJECT_NAME} animation solver_cuda)
endif()

if (USE_OpenMP)
    add_dependencies(${PROJECT_NAME} animation solver_omp)
    target_link_libraries(${PROJECT_NAME} animation solver_omp)
endif()

if (USE_QT)
    target_link_libraries(
        ${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        )
endif()


set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>
)
