cmake_minimum_required(VERSION 3.19)
project(Stonks-Game CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES Game/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES Game/include/*.h)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})

# Common libs for all platforms
set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/Lib/Common)

# Specific libs
if (WIN32) 
	set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/Lib/Windows)
	include_directories(main ${CMAKE_PREFIX_PATH}/PDCurses-3.9)
	target_link_libraries(main PUBLIC ${CMAKE_PREFIX_PATH}/pdcurses.lib)
elseif (UNIX)
	set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/Lib/Linux)
	target_link_libraries(main PUBLIC ncurses)
endif()

add_subdirectory(Console_GUI)

include_directories(Console_GUI/include)

target_link_libraries(main PRIVATE Console_GUI)

