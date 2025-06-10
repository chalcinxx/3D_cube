cmake_minimum_required(VERSION 3.10)
project(release VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

find_package(cx REQUIRED)
find_package(SFML 2.6 COMPONENTS window system graphics audio REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${cx_LIBRARIES} sfml-window sfml-system sfml-graphics sfml-audio)
