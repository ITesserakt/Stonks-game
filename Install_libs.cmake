# Beware dark magic!

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/danielaparker/jsoncons
        GIT_TAG f85537d49b5f6f907f5e05d097350596676c2a26)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929)

FetchContent_Declare(ranges-v3
        GIT_REPOSITORY https://github.com/ericniebler/range-v3
        GIT_TAG 4989f3e9ff2efee1852942bb9328ef121369ba02)

FetchContent_Declare(terminal
        GIT_REPOSITORY https://github.com/jupyter-xeus/cpp-terminal
        GIT_TAG 863e5e5eccab006a7dd7987924f834638cd0ddd0)

FetchContent_MakeAvailable(googletest json ranges-v3 terminal)

# Phew, dark magic ended!

find_library(Curses NAMES ncurses curses pdcurses REQUIRED)
link_libraries(pthread)
