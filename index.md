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

We implemented simple but flexible `TUI` library to construct this game.

There are `widgets` - parts of terminal with special behaviour.

### Hierarchy 

```text
- Widget                  Main and the most simple kind of widgets
|- BindableWidget         Widgets that can have children tree (canvas for example)
|- ColorWidget            Widgets with color property
|\ - SizeableWidget       Widgets with size property
| \_\- PositionedWidget   Widgets with position; can also be hidden
|      \- HowerableWidget Widgets that can be clicked
```

Built-in widgets: `canvas, button, label, message box, graphic, group`.

### DSL

#### Commands

There are various commands to interact with buttons.

For example:

```c++
Button button{/* name: */ "Example", /* tab index */ 0};
button.applyAction(SceneChangedCommand(...)); // when user clicks on this button, act method will be called
```

We can even chain simple commands to build more complex.

> Note: every command `A` that can be chained should extends `CloneCommand<A>` class

```c++
auto complex = HideCommand(...) + HideCommand(...) + KillSomeoneCommand(...)
```
#### Layout builder

Calling `auto button = std::make_shared<Button>(...)` is pretty noisy and boring.

This can lead to very complicated and hard to read code.

The solution is `layout builder`!

This is a set of lightweight wrappers to eliminate boiler-plate code.

```c++
using namespace widget;

canvas {
  button { /* constructor of button */ },
  label { ... },
  group { ...,
    label { ... },
    message_box { ... },
  },
  // etc
}
```

Will make `Canvas` with button, label and group

Among other wrappers, there are `many` that wraps creation of multiple widgets:

```c++
canvas {
  many<Label> { /* count */, /* Label(std::size_t) */ } // parameter of a function is ordinal number of widget
}
```

To append widgets layout to existing canvas, use `canvas{ /* some existing canvas */ }.append(/* layout */)`

> See `Console_GUI/widgets/dsl/DSL.h` for more information.

***

But what if you need to setup label color or button's action?

We don't have any variables, so how it's possible?

The answer is `widget setup`

```c++
using namespace widget;
using namespace widget::setup;

canvas {
  label { /* constructor parameters */, color(...) },
  // or...
  label { ... } << color(...),
  button { ... } << command</* type */> (/* constructor parameters */) + command<...>(...)
  // syntax many<...> { /* count */, /* fn */, /* a lot of setups */ } isn't supported
}
```
***

Ok, but what if we need self widget in setup? Or even a reference to other widget in current canvas?

`widget implicits`!

```c++
using namespace widget;
using namespace widget::setup;
using namespace widget::implicit;

canvas {
  button { ... } << command<...>(self<Button>(), ...), // self will be resolved to button instance
  group { "Very cool name" },
  button { ... } command<...>(search<Group>("Very cool name"), ...) // search will be resolved to group right instance
  // make sure you are searching real instances of widgets, not wrappers!
}
```

`search` has 3 types: `Root`, `FirstBindable` and `Bindable`.

* `Root` - search down from canvas (used by default)
* `FirstBindable` - search down from the first for current widget bindable
* `Bindable` - specify bindable to search from by name
