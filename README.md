# C++ Addressbook Class
## Requirements
 - CMake 3.20 or higher
 - A compiler supporting C++20. e.g.
    - GCC 8+
    - Visual Studio 2022+

## Building 
```BASH
    cmake -S . -B out
    cmake --build out
```
or use an IDE with built in cmake support

## Testing
```BASH
    cd out
    ctest --output-on-failure 
```
