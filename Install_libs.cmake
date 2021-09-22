# Beware dark magic!

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG 626e7d61e44dee32887126c8f437dd077dec09cf)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929
)

FetchContent_MakeAvailable(googletest json)

# Phew, dark magic ended!

find_library(Curses NAMES ncurses curses pdcurses REQUIRED)
