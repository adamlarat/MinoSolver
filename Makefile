FLAG=-g -Wall -Wextra -pedantic
CC=g++
FLAG=-Wall -O1
EXEC=minos.exe
all: exec
debug: exec
debug: FLAG=-Wall -g3 -D DEBUG -D VERBOSE_LEVEL=3
debug: EXEC=minos-debug.exe
release: exec
release: FLAG=-O3 -Wall
release: EXEC=minos-release.exe
main.o: main.cpp dlx.hpp mino.hpp
	$(CC) $(FLAG) -c main.cpp
mino.o: mino.hpp mino.cpp
	$(CC) $(FLAG) -c mino.cpp
dlx.o: dlx.hpp dlx.cpp
	$(CC) $(FLAG) -c dlx.cpp
exec: mino.o dlx.o main.o
	$(CC) $(FLAG) main.o mino.o dlx.o -o $(EXEC)
clean:
	rm minos*.exe *.o
