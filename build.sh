#!/bin/bash

LOG_COMPILE_PATH="build.log"
ARCHIVE_PATH="../archive/"

make clean
make CROSS_COMPILE=$CROSS_COMPILE
