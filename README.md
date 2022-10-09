# SDL2-Tetris
Learning sdl2 and CMake, Out-of-Source Build
> Refactoring [SDL2-GameDev/Tetris](https://github.com/guyllaumedemers/SDL2-GameDev/tree/master/SDL2-Tetris)

## Overview
> SDL2-Tetris is a demo project set using Modern CMake & ImGUI.

## Issues
```
CMake Error at src/appTetris/CMakeLists.txt:138 (message):
SEND_ERROR! TARGET TRY COMPILATION FAILED :
D:/VisualStudio/SDL2/SDL2-GameDevCMake/SDL2-Tetris/src/appTetris/Main.cpp,
COMPILE OUTPUT : Change Dir:
D:/VisualStudio/SDL2/SDL2-GameDevCMake/SDL2-Tetris/out/build/bin/SDL2-TetrisApp/test/CMakeFiles/CMakeTmp

Run Build Command(s):C:/Program Files/Microsoft Visual
Studio/2022/Enterprise/MSBuild/Current/Bin/amd64/MSBuild.exe
cmTC_4786f.vcxproj /p:Configuration=Debug /p:Platform=x64
/p:VisualStudioVersion=17.0 /v:m && MSBuild version 17.3.1+2badb37d1 for
.NET Framework

Microsoft (R) C/C++ Optimizing Compiler Version 19.33.31629 for x64
Main.cpp
Copyright (C) Microsoft Corporation.  All rights reserved.
cl /c /Zi /W1 /WX- /diagnostics:column /Od /Ob0 /D _MBCS /D WIN32 /D _WINDOWS /D "CMAKE_INTDIR=\"Debug\"" /Gm- /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Fo"cmTC_4786f.dir\Debug\\" /Fd"cmTC_4786f.dir\Debug\vc143.pdb" /external:W1 /Gd /TP /errorReport:queue "D:\VisualStudio\SDL2\SDL2-GameDevCMake\SDL2-Tetris\src\appTetris\Main.cpp"

LINK : fatal error LNK1104: cannot open file 'tryRunlibImGUI.lib'
[D:\VisualStudio\SDL2\SDL2-GameDevCMake\SDL2-Tetris\out\build\bin\SDL2-TetrisApp\test\CMakeFiles\CMakeTmp\cmTC_4786f.vcxproj]
```

💬 References on CMake are given under : [Modern CMake for C++](https://www.amazon.ca/Modern-CMake-Discover-approach-packaging/dp/1801070059), [Mastering CMake](https://www.amazon.ca/Mastering-CMake-Ken-Martin/dp/1930934319)
