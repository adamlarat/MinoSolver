FLAG=-g -Wall -Wextra -pedantic
CC=g++
FLAG=-Wall -O1
EXEC=minos.exe
all: exec
debug: exec
debug: FLAG=-Wall -g3 -D TRACK -D VERBOSE_LEVEL=3
debug: EXEC=minos-debug.exe
release: exec
release: FLAG=-O3 -Wall
release: EXEC=minos-release.exe
fast: exec
fast: FLAG=-O3 -Wall -D FAST
fast: EXEC=minos-fast.exe
myMain.o: myMain.cpp myDLX.hpp mino.hpp
	$(CC) $(FLAG) -c myMain.cpp
mino.o: mino.hpp mino.cpp
	$(CC) $(FLAG) -c mino.cpp
myDLX.o: myDLX.hpp myDLX.cpp
	$(CC) $(FLAG) -c myDLX.cpp
exec: mino.o myDLX.o myMain.o
	$(CC) $(FLAG) myMain.o mino.o myDLX.o -o $(EXEC)
clean:
	rm -f minos*.exe *.o *.gch
