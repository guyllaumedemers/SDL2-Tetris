# SDL2-Tetris
Learning sdl2 and CMake, Out-of-Source Build
> Refactoring [SDL2-GameDev/Tetris](https://github.com/guyllaumedemers/SDL2-GameDev/tree/master/SDL2-Tetris)

## Overview
> SDL2-Tetris is a demo project set using Modern CMake & ImGUI.

## Issues
```
Main.obj : error LNK2019: unresolved external symbol "void __cdecl
libImGUITest(void)" (?libImGUITest@@YAXXZ) referenced in function main
[D:\VisualStudio\SDL2\SDL2-GameDevCMake\SDL2-Tetris\out\build\bin\SDL2-TetrisApp\test\CMakeFiles\CMakeTmp\cmTC_e57e8.vcxproj]

Main.obj : error LNK2019: unresolved external symbol "void __cdecl
libSDL2Test(void)" (?libSDL2Test@@YAXXZ) referenced in function main
[D:\VisualStudio\SDL2\SDL2-GameDevCMake\SDL2-Tetris\out\build\bin\SDL2-TetrisApp\test\CMakeFiles\CMakeTmp\cmTC_e57e8.vcxproj]
```

💬 References for patterns and physical desing are given from : [Design Patterns: Elements of Reusable Object‑Oriented Software](https://www.amazon.ca/-/fr/Gamma-Erich-ebook/dp/B000SEIBB8), [Large-Scale-Software-Design-John-Lakos](https://www.amazon.ca/Large-Scale-Software-Design-John-Lakos/dp/0201633620)
