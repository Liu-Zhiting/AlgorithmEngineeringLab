DIR_SRC = .
DIR_BIN = .

SRC = $(wildcard *.cpp)

TARGET = triangleCounting

BIN_TARGET = ${DIR_BIN}/${TARGET}
 
CC = clang++
# CC = g++

CFLAGS = -g -fcilkplus
# CFLAGS = -fcilkplus -O2

${BIN_TARGET}:${SRC}
	$(CC) $(CFLAGS) $(SRC) -o ${BIN_TARGET}