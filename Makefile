CC = gcc
CC_FLAGS += -Wall
CC_FLAGS += -O3
CC_FLAGS += -g3
#CC_FLAGS += -fstack-protector-strong

LIBS += -lGL
LIBS += -lGLU
LIBS += -lglut

glem:
	$(CC) $(CC_FLAGS) main.c $(LIBS) -o glem.x

all: glem client

client:
	$(CC) $(CC_FLAGS) clientSample.c $(LIBS) -o client

.PHONY: clean client glem glem.x
clean:
	-rm glem.x client
