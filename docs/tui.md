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
