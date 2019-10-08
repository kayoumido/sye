#!/bin/bash
LOG_RUN_PATH="run.log"
ARCHIVE_PATH="./archive/"

echo "======== Run no $1: $2 =========" | tee -a $ARCHIVE_PATH$LOG_RUN_PATH
./st | tee -a $ARCHIVE_PATH$LOG_RUN_PATH
