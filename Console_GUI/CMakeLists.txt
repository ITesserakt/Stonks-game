file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_library(Console_GUI ${SOURCE_FILES} ${HEADER_FILES})

include_directories("/usr/local/Cellar/tbb/2021.3.0/include")

target_include_directories(Console_GUI
		PUBLIC    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		INTERFACE $<INSTALL_INTERFACE:include>)
target_link_libraries(Console_GUI PUBLIC ncurses)
# set(CMAKE_CXX_FLAGS -L/usr/local/Cellar/tbb/2021.3.0/include)
