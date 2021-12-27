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