CC = gcc
MKDIR = mkdir
OBJDIR = bin
CC_FLAGS := -Wall -O3 -g3
#CC_FLAGS := -fstack-protector-strong

LIBS := -lGL -lGLU -lglut

all: glem sample

glem: bindir
	$(CC) $(CC_FLAGS) server.c $(LIBS) -o $(OBJDIR)/server.o

bindir:
	$(MKDIR) -p $(OBJDIR)

sample:
	@cd sample/ && make
	@mv sample/*.o $(OBJDIR)/

.PHONY: clean client glem sample
clean:
	@cd sample && make clean
	@rm -rf bin/
