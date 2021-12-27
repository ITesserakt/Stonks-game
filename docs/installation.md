## Installation

Check the releases tab to catch the last release

Or, you can build a project by yourself:

```bash
git clone https://github.com/ITesserakt/Stonks-game
cd Stonks-game
mkdir build
cd build
cmake -B . -S .. -DCMAKE_BUILD_TYPE=Debug
make main
```

Here we go!

To run the game, type this from `build` folder:

```bash
cd bin
./main
```

You can also install the game to the specified location just put `-DCMAKE_PREFIX_PATH=...` like this: `cmake -B . -S .. -DCMAKE_PREFIX_PATH=/home/` and then `make install`

## Requirements

* `Linux` or `Mac OS`
* `cmake >= 3.6`
* `ncurses`
* Internet connection (to download neccessary libs)
* C++ compiler with at least C++17 standard support
