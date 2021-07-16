DIR_SRC = .
DIR_BIN = .

SRC = $(wildcard *.cpp)

TARGET = sort

BIN_TARGET = ${DIR_BIN}/${TARGET}
 
CC = clang++
CFLAGS = -g -fcilkplus

${BIN_TARGET}:${SRC}
	$(CC) $(CFLAGS) $(SRC) -o ${BIN_TARGET}