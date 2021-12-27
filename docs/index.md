# Stonks-game

## Description

Stonks Game is an exchange trading simulator. Here you should buy items and sell them at a new price. If your balance drops below 0, you lose. You win if you earn enough money depending on the current difficulty.

Also, there are several bots - little AIs which will mess you.

In the `shared` folder, you can find game config, save, and other stuff. Feel free to edit these things as you wish, but remember, you have to remove save after every config change.

***

> This project made for the "Educational practice on C++" course at Bauman Moscow State Technical University

## Installation

Check the releases tab to catch the last release

Or, you can build project by yourself:

```bash
git clone https://github.com/ITesserakt/Stonks-game
cd Stonks-game
mkdir build
cmake -B . -S ..
make main
```

Here we go!

To run the game type this from `build` folder:

```bash
cd bin
./main
```

You can also install the game to the specified location, just put `-DCMAKE_PREFIX_PATH=...` like this: `cmake -B . -S .. -DCMAKE_PREFIX_PATH=/home/`

### Requirements

* `Linux` or `Mac OS`
* `cmake >= 3.6`
* `ncurses`
* Internet connection (to download neccessary libs)
* C++ compiler with at least C++17 standard support

## Key bindings

Text below also located in the `guide` tab in the game

| Key   | Action  |
| ----- | ------- |
| &#8593; | go higher |
| &#8595; | go lower |
| Enter | button click |
| &#8592;/&#8594; | change item price by 10 points |

## Config

Config is a file, located in `build/share/config.json`

* `botsAmount` - amount of AIs simultaneously working in the game
* `debug` and `debugSpeedGame` - yep, debug session, don't turn it, if you don't want to get a lot of bugs
* `difficulty` - current difficulty. Can be set in game at the settings tab
* `maxFPS` - maximum frames per second
* `worldSize` - total items amount
* `presets` - difficulty levels
   * `initialMoney` - money, that receives player when game starts
   * `inventorySize` - amount of items which can be purchased by player at the same time
   * `taxPercentage` - taxes; tick every 10 seconds
   * `winCondition` - you win if your balance above this number

## TUI library

> See [TUI page](tui.md) for more information