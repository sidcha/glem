#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    File: glem/Makefile
#    Created: 07-May-2016
#    Author: Siddharth Chandrasekaran <siddharth@embedjournal.com>

CC_FLAGS     += -Wall -O3
INCLUDE_PATH += -Iinclude
LIB_PATH     += -Llib

ifeq ($(OS),Windows_NT)
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
    CCFLAGS       += -D WIN32
    LDFLAGS       += -Wl,--subsystem,windows
    INCLUDE_PATH  += -I"C:\Program Files\freeglut\include"
    LIB_PATH      += -L"C:\Program Files\freeglut\lib\x64"
    LIBS          := -lfreeglut -lopengl32
    GLEM_BIN      := Glem.exe
    EXAMPLE_BIN   := Example.exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
    LIBS        := -lGL -lGLU -lglut
    GLEM_BIN    := glem
    EXAMPLE_BIN := example
endif

CC       := gcc
LIBS     += -lglem -pthread

all: dirs lib/libglem.a glem examples

glem: obj/glem.o
	@echo "Building glem server"
	@$(CC) $(LDFLAGS) -o $(GLEM_BIN) $^ $(LIB_PATH) $(LIBS)

lib/libglem.a: obj/libglem.o
	@echo "Building libglem.a"
	@ar rcs lib/libglem.a obj/libglem.o

examples:
	@make -s -C examples all

manpages:
	@echo "Regenerating man pages from markdown"
	@ronn  -r doc/glem.ronn doc/lib-glem.ronn

obj/%.o: src/%.c
	@echo "Compiling $<"
	@$(CC) $(CCFLAGS) -o "$@" -c "$<" $(INCLUDE_PATH)

dirs:
	@mkdir -p obj lib

clean:
	@make -s -C examples clean
	@rm -rf obj/ lib/ $(GLEM_BIN)

.PHONY: clean glem dirs manpages examples
