file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

enable_testing()

add_executable(Game_tests ${TEST_FILES}
        src/Config.cpp
        src/CreatingGui.cpp
        src/ObjectFactory.cpp
        src/AI.cpp
        src/Debug.cpp
        src/EventHandler.cpp
        src/FramePainter.cpp
        src/GameObject.cpp
        src/Gamer.cpp
        src/Player.cpp
        src/World.cpp
        src/GameWidgets/ButtonWithObj.cpp
        src/GameWidgets/Purchase.cpp
        src/GameWidgets/Sale.cpp)

target_include_directories(Game_tests PRIVATE include)
target_link_libraries(Game_tests PRIVATE Console_GUI gtest_main jsoncons gmock)