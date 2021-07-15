DIR_SRC = .
DIR_BIN = .

SRC = $(wildcard *.cpp)

TARGET = matrixMul

BIN_TARGET = ${DIR_BIN}/${TARGET}
 
CC = clang++
CFLAGS = -fcilkplus -g

${BIN_TARGET}:${SRC}
	$(CC) $(CFLAGS) $(SRC) -o ${BIN_TARGET}