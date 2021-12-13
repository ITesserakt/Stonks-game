cmake_minimum_required(VERSION 3.6)
project(Stonks-Game CXX)
set(CMAKE_CXX_STANDARD 17)

set(BUILD_TESTS OFF)
set(JSONCONS_BUILD_TESTS OFF)
set(CPPTERMINAL_BUILD_EXAMPLES OFF)
set(CPPTERMINAL_ENABLE_TESING OFF)

include(Install_libs.cmake)

file(GLOB_RECURSE SOURCE_FILES Game/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES Game/include/*.h)
file(GLOB_RECURSE RESOURCES Game/resources/*)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

add_subdirectory(Console_GUI)
add_subdirectory(Game)

target_include_directories(main PRIVATE Console_GUI/include)
target_include_directories(main PRIVATE Game/include)

target_link_libraries(main PRIVATE Console_GUI jsoncons range-v3)

install(FILES ${RESOURCES}
        TYPE DATA)
install(TARGETS main
        TYPE RUNTIME)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/share)
foreach(item IN LISTS RESOURCES)
    configure_file(${item} ${CMAKE_BINARY_DIR}/share COPYONLY)
endforeach()