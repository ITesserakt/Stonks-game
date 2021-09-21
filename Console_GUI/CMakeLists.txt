file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

add_library(Console_GUI ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Console_GUI
		PUBLIC    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		INTERFACE $<INSTALL_INTERFACE:include>)

# Dark magic!
include(FetchContent)
FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929
)
FetchContent_MakeAvailable(googletest)
# Dark magic ended!

enable_testing()
add_executable(tests ${TEST_FILES} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(tests PRIVATE include)
target_link_libraries(tests gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)