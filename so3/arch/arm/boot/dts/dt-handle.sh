#!/bin/bash 
# Forced to bash. Ubuntu variants have dash as default shell !
# 
# Script to manipulate device trees (compile, pre-process, ...)
# 

# $1 = board
# $2 = action (compile, pre-process, reverse compile, clean)

if [ "$#" -ne 2 ]; then
    echo "Usage: dtc-handle <boardname> <command>"
    echo "		 supported commands: compile|preproc|reverse|clean"
    exit 1
fi

SRC_DIR=$( dirname $( readlink -f "$0") )
DEST_DIR=$PWD

BOARD=$1
BOARD_DTS=$SRC_DIR/$BOARD.dts


# Use CROSS_COMPILE if set and not an empty string
if [ -z "$CROSS_COMPILE" ]; then
	CC=arm-linux-gnueabihf-gcc;
else
	CC=${CROSS_COMPILE}gcc;
fi

# Exit if only pre-processing is wanted
if [ "$2" = "preproc" -o "$2" = "compile" ]; then
	
	if [ ! -f $BOARD_DTS ]; then
		echo "$BOARD_DTS not found !"
		exit 1
	fi

	# Pre-process the .dts file to generate the expanded complete dts
	$CC -P -E -nostdinc -I. -undef -D__DTS__ -x assembler-with-cpp -o $SRC_DIR/$BOARD-preprocessed.dts $BOARD_DTS
	echo -e "\tPre-process:\tdts file generated in $SRC_DIR/$BOARD-preprocessed.dts"
	if [ "$2" = "preproc" ]; then
		exit 0
	fi
fi

# Compile the device tree into binary (blob) format
if [ "$2" = "compile" ]; then
	dtc $SRC_DIR/$BOARD-preprocessed.dts -O dtb -o $DEST_DIR/$BOARD.dtb
	echo -e "\tCompile:\t$BOARD_DTS compiled into $DEST_DIR/$BOARD.dtb"
fi

# Reverse 
if [ "$2" = "reverse" ]; then
	
	if [ ! -f $DEST_DIR/$1.dtb ]; then
		echo "$DEST_DIR/$BOARD.dtb not found !"
		exit 1
	fi

	dtc -I dtb -O dts -o $DEST_DIR/$BOARD-reversed.dts $DEST_DIR/$BOARD.dtb
	echo -e "\tReverse-compile:\t$DEST_DIR/$BOARD.dtb reverse-compiled into $DEST_DIR/$BOARD-reversed.dts"
fi

# Clean all generated files keeping only dts/dtsi sources
if [ "$2" = "clean" ]; then
	rm -f $SRC_DIR/$BOARD-preprocessed.dts $DEST_DIR/$BOARD.dtb $DEST_DIR/$BOARD-reversed.dts
	echo -e "\tCleanup:\tGenerated files for board '$BOARD' cleaned."
fi

echo "Done."
