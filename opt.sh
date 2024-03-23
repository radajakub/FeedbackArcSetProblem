#!/bin/bash

make

./build/optimal ./data/instances/$1.txt ./$1.out
