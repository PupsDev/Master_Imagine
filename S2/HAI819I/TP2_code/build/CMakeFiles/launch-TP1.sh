#!/bin/sh
bindir=$(pwd)
cd /home/e20150011037/Documents/Master_Imagine-main/S2/HAI819I/TP2_code/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20150011037/Documents/Master_Imagine-main/S2/HAI819I/TP2_code/build/TP1 
	else
		"/home/e20150011037/Documents/Master_Imagine-main/S2/HAI819I/TP2_code/build/TP1"  
	fi
else
	"/home/e20150011037/Documents/Master_Imagine-main/S2/HAI819I/TP2_code/build/TP1"  
fi
