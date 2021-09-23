file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

enable_testing()

add_executable(Game_tests ${HEADER_FILES} ${TEST_FILES}
        src/ObjectFactory.cpp src/World.cpp src/AI.cpp src/GameObject.cpp)

target_include_directories(Game_tests PRIVATE include)
target_link_libraries(Game_tests PRIVATE gtest_main nlohmann_json::nlohmann_json ncurses)