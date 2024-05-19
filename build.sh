#!/bin/sh
set -e
CC=gcc
CFILES="path.c history.c line.c cmd.c main.c readline.c data_structers/*.c"
CFLAGS="-O0 -g"
EXENAME="exe"

compile()
{
	$CC $CFLAGS $1 $CFILES -o exe
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
else
	compile
	echo "build succsesfuly!"
fi
