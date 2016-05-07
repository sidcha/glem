CC = gcc
CC_FLAGS += -Wall
LIBS += -lGL
LIBS += -lGLU
LIBS += -lglut

all: glem client

glem:
	$(CC) $(CC_FLAGS) main.c $(LIBS) -o glem 

client:
	$(CC) $(CC_FLAGS) clientSample.c $(LIBS) -o client

install: glem
	install glem /usr/local/bin

.PHONY: clean client glem
clean:
	-rm glem client
