#!/bin/bash

make opt

./build/optimal ./data/instances/$1.txt ./$1.out
