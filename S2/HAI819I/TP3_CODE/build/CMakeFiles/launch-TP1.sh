#!/bin/sh
bindir=$(pwd)
cd /home/e20150011037/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20150011037/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1 
	else
		"/home/e20150011037/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1"  
	fi
else
	"/home/e20150011037/Documents/Master_Imagine/S2/HAI819I/TP3_CODE/build/TP1"  
fi
