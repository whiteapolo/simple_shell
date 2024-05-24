#!/bin/sh
set -e
CC=cc
# CFILES="path.c history.c line.c cmd.c main.c readline.c data_structers/*.c"
CFILES="*.c data_structers/*.c"
CFLAGS="-O3 -g"
# EXENAME="mysh"
EXENAME="exe"

compile()
{
	$CC $CFLAGS $1 $CFILES -o $EXENAME
}

profile()
{
	compile -pg
	./$EXENAME
	gprof ./$EXENAME gmon.out > $1
}

clean()
{
	rm $EXENAME
}

if [ "$1" = "clean" ]; then
	clean
elif [ "$1" = "profile" ]; then
	if [ -n "$2" ]; then
		profile $2
		echo "profile: $2!"
	else
		echo "file name required!"
	fi
elif [ "$1" = "release" ]; then
	compile
	mv $EXENAME /home/white/.local/bin/mysh
else
	compile
	echo "build succsesfuly!"
fi
