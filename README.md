An implementation of [breakout](https://en.wikipedia.org/wiki/Breakout_(video_game)) in x86 assembly, C and C++.

# Configure and Build
## Linux

`$ cmake -S . -B build`\
`$ cmake --build build --parallel 8`

Optionally you could configure for Release/Debug builds\
`$ cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=DEBUG`\
`$ cmake -S . -B build/release -DCMAKE_BUILD_TYPE=RELEASE`

## Windows

`c:\> cmake -S . -B build`\
`c:\> cmake --build build --config Release`

Or you can just open the solution and build in the GUI\
`c:\> start build\asm_c_cpp.sln`
