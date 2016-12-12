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
CC_FLAGS := -Wall -O3 -g3
LIBS     := -lGL -lGLU -lglut

GLEM_OBJ := obj/glem-server.o
SAMPLE_OBJ := obj/glem-client.o obj/glcd.o obj/fonts.o obj/symbols.o

all: dirs glem sample

obj/%.o: src/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" -c "$<"

glem: $(GLEM_OBJ)
	@$(CC) $(CC_FLAGS) -o bin/$@.elf $^ $(LIBS)

sample: $(SAMPLE_OBJ)
	@$(CC) $(CC_FLAGS) -o bin/$@.elf $^ $(LIBS)

dirs:
	@mkdir -p bin
	@mkdir -p obj

clean:
	@rm -rf bin/* obj/*

.PHONY: clean glem sample

