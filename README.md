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

💬 References for general c++ knowledge and optimization of physical desing are given from : [C++ Programming Language](https://www.amazon.ca/C-Programming-Language-4th/dp/0321563840/ref=sr_1_6?crid=1NJCZVP0FQ11N&keywords=c%2B%2B+programming&qid=1668273337&qu=eyJxc2MiOiI0Ljk0IiwicXNhIjoiNC4yNSIsInFzcCI6IjMuOTUifQ%3D%3D&sprefix=c%2B%2B+programming%2Caps%2C88&sr=8-6), [Large-Scale-Software-Design-John-Lakos](https://www.amazon.ca/Large-Scale-Software-Design-John-Lakos/dp/0201633620)</br>
💬 References on CMake are given under : [Modern CMake for C++](https://www.amazon.ca/Modern-CMake-Discover-approach-packaging/dp/1801070059), [Mastering CMake](https://www.amazon.ca/Mastering-CMake-Ken-Martin/dp/1930934319)
