#!/bin/bash
mkdir build_win
cd build_win

cmake .. -G "Visual Studio 16 2019" -Ax64 -DTARGET_LANGUAGE=python
cmake --build . --config Release
