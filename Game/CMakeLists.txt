file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

enable_testing()

add_executable(Game_tests ${TEST_FILES}
        src/game_widgets/Purchase.cpp
        src/game_widgets/Sale.cpp
        src/game_widgets/SharedGraphic.cpp
        src/game_widgets/TradeButton.cpp
        src/AI.cpp
        src/Config.cpp
        src/CreatingGui.cpp
        src/Debug.cpp
        src/EventHandler.cpp
        src/FramePainter.cpp
        src/GameLoading.cpp
        src/GameObject.cpp
        src/Gamer.cpp
        src/ObjectFactory.cpp
        src/Player.cpp
        src/Statistics.cpp
        src/World.cpp
        src/WorldState.cpp)

target_include_directories(Game_tests PRIVATE include)
target_link_libraries(Game_tests PRIVATE Console_GUI gtest_main jsoncons gmock range-v3)