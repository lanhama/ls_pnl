#!/bin/bash

cd build
rm -rf *
cmake ..
make
./pnl_calculator