CC = gcc
CC_FLAGS += -Wall
LIBS += -lGL
LIBS += -lGLU
LIBS += -lglut

SRCS = main.c

glem:
	$(CC) $(CC_FLAGS) $(SRCS) $(LIBS) -o glem 
install: glem
	install glem /usr/local/bin

.PHONY: clean
clean:
	-rm glem
