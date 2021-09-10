cmake_minimum_required(VERSION 3.19)
project(Stonks-Game CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES Game/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES Game/include/*.h)

add_subdirectory(Console_GUI)

include_directories(Console_GUI/include)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES} Console_GUI/src/Widget.cpp)
target_link_libraries(main PRIVATE Console_GUI ncurses)

