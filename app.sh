#!/bin/bash

make app

./build/approximate ./data/instances/$1.txt ./$1.out
