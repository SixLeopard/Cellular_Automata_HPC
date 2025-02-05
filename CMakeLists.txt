cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Cellular_Automata LANGUAGES CXX CUDA)
enable_language(CUDA)

include(CheckLanguage)
check_language(CUDA)

set (CMAKE_CXX_STANDARD 20)

#uncomment below before compling with CUDA enabled
set_source_files_properties(main.cpp PROPERTIES LANGUAGE CUDA)

find_package(Curses REQUIRED)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-O3 -fopenmp")
SET(CMAKE_CUDA_COMPILER nvcc)

find_package(OpenMP)
find_package(MPI)

include_directories(${CURSES_INCLUDE_DIR} ${MPI_INCLUDE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

if(OpenMP_CXX_FOUND)
  target_link_libraries(${PROJECT_NAME}
    # OpenMP::OpenMP_CXX is an imported target with INTERFACE_**
    $<$<CONFIG:Release>:OpenMP::OpenMP_CXX> )
endif()

add_custom_target(copy_resources1 ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
        COMMENT "Copying resources into binary directory")

add_custom_target(copy_resources2 ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/results
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/results
        COMMENT "Copying resources into binary directory")

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} ${MPI_LIBRARIES})
add_dependencies(${PROJECT_NAME} copy_resources1)
add_dependencies(${PROJECT_NAME} copy_resources2)

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()