############################################################################
#                                                                          #
#  This program is free software: you can redistribute it and/or modify    #
#  it under the terms of the GNU General Public License as published by    #
#  the Free Software Foundation, either version 3 of the License, or       #
#  (at your option) any later version.                                     #
#                                                                          #
#  This program is distributed in the hope that it will be useful,         #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of          #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           #
#  GNU General Public License for more details.                            #
#                                                                          #
#  You should have received a copy of the GNU General Public License       #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                          #
############################################################################
#
#
#        File: glem/src/glcd.c
#  Created on: 07-May-2016
#      Author: Siddharth Chandrasekaran
#        Mail: siddharth@embedjournal.com
#

CC       := gcc
CC_FLAGS := -Wall -O3
LIBS     := -lGL -lGLU -lglut

SAMPLE_OBJ := 

all: dirs glem

glem: obj/glem.o obj/glcd.o obj/fonts.o obj/symbols.o
	@$(CC) $(CC_FLAGS) -o $@ $^ $(LIBS)

install: glem
	mkdir -p /usr/local/bin
	install -m 755 glem /usr/local/bin

uninstall:
	@rm -rf /usr/local/bin/glem

obj/%.o: src/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

dirs:
	@mkdir -p obj

clean:
	@rm -rf obj glem

.PHONY: clean glem glem-test

