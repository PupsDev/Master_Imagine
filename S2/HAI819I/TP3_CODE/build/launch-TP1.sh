#!/bin/sh
bindir=$(pwd)
cd /home/pups/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/pups/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1 
	else
		"/home/pups/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1"  
	fi
else
	"/home/pups/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1"  
fi
