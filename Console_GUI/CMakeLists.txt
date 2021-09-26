file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

add_library(Console_GUI ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Console_GUI
		PUBLIC    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		INTERFACE $<INSTALL_INTERFACE:include>)
target_link_libraries(Console_GUI ncurses)

enable_testing()
add_executable(GUI_test ${TEST_FILES} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(GUI_test  PRIVATE include)
target_link_libraries(GUI_test gtest_main ncurses)