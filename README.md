# SDL2-Tetris
Learning sdl2 and CMake, Out-of-Source Build
> Refactoring [SDL2-GameDev/Tetris](https://github.com/guyllaumedemers/SDL2-GameDev/tree/master/SDL2-Tetris)

## Overview
> SDL2-Tetris is a demo project built using Modern CMake. Project Optimization is under development and currently favor RValue Reference for storing RValue literals and Object requiring Move semantics. LValue Reference are currently favor for Heap allocated variables.

#### Note : Assembly instructions call comparisons are required to confirm the improvement of code efficiency.

## How to Run
> To Run this project and generate MSVC solution, open powershell and run the following cmd from the project root directory.</br>
```.\config.cmd``` ```.\build.cmd``` ```.\run.cmd```

#### Note : CMake setup expect proper ENV PATH setup on your machine.

💬 References on CMake are given under : [Modern CMake for C++](https://www.amazon.ca/Modern-CMake-Discover-approach-packaging/dp/1801070059), [Mastering CMake](https://www.amazon.ca/Mastering-CMake-Ken-Martin/dp/1930934319)
