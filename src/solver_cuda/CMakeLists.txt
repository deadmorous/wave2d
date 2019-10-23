cmake_minimum_required (VERSION 3.1.3) 

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${ProjectId} LANGUAGES CUDA)

file(GLOB HEADER_FILES
    ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp
    ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.h
)

aux_source_directory(. SOURCE_FILES)
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
  COMPILE_DEFINITIONS $<$<CONFIG:Debug>:_DEBUG>
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
