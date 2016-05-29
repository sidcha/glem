CC = gcc

CC_FLAGS += -Wall
CC_FLAGS += -O3
CC_FLAGS += -g3
#CC_FLAGS += -fstack-protector-strong

sample:
	$(CC) glcd.c main.c -o sample.o

.PHONY: clean
clean:
	-rm sample.o
