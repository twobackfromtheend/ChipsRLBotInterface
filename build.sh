#!/bin/bash
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -DTARGET_LANGUAGE=both
make -j10
