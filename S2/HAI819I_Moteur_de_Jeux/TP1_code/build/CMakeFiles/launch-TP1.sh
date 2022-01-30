#!/bin/sh
bindir=$(pwd)
cd /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		GDB_COMMAND-NOTFOUND -batch -command=$bindir/gdbscript  /home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/TP1 
	else
		"/home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/TP1"  
	fi
else
	"/home/pups/Documents/Master_Imagine/S2/HAI819I_Moteur_de_Jeux/TP1_code/build/TP1"  
fi
