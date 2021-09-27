cmake_minimum_required(VERSION 3.19)
project(Stonks-Game CXX)
set(CMAKE_CXX_STANDARD 17)

include(Install_libs.cmake)

file(GLOB_RECURSE SOURCE_FILES Game/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES Game/include/*.h)

add_compile_options(-Wall -Wextra)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

add_subdirectory(Console_GUI)
add_subdirectory(Game)

target_include_directories(main PRIVATE Console_GUI/include)
target_include_directories(main PRIVATE Game/include)

target_link_libraries(main PRIVATE Console_GUI nlohmann_json::nlohmann_json ncurses)