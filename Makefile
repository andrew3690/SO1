CC = g++
CFLAGS = -g -Wall -std=c++17 -D_XOPEN_SOURCE
#INCLUDES = cpu.cc debug.cc main.cc main_class.cc system.cc thread.cc semaphore.cc
INCLUDES = *.cc
OBJ = main

main:
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS}

clean:
	rm -f main