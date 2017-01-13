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
CC_FLAGS := -Wall -O3 -Iinc -Llib
LIBS     := -lGL -lGLU -lglut
INST_PATH:= /usr/local
MAN_PATH := ${INST_PATH}/share/man
ELF      := glem


all: dirs libglem glem 


glem: obj/glem.o obj/passfd.o
	@$(CC) $(CC_FLAGS) -o $@ $^ $(LIBS) -lglem


libglem: obj/libglem.o
	@ar rcs lib/libglem.a obj/libglem.o


install: glem
	@mkdir -p ${INST_PATH}/bin
	@mkdir -p ${INST_PATH}/include
	@mkdir -p ${INST_PATH}/lib
	@mkdir -p ${MAN_PATH}/man1
	@mkdir -p ${MAN_PATH}/man3
	@install -m 755 ${ELF} ${INST_PATH}/bin
	@cp lib/libglem.a ${INST_PATH}/lib
	@cp inc/glem.h ${INST_PATH}/include
	@cp doc/glem.1 ${MAN_PATH}/man1/
	@cp doc/lib-glem.3 ${MAN_PATH}/man3/


uninstall:
	@rm -rf ${INST_PATH}/bin/${ELF}
	@rm -rf ${INST_PATH}/lib/libglem.a
	@rm -rf ${INST_PATH}/include/glem.h
	@rm -rf ${MAN_PATH}/man1/glem.1
	@rm -rf ${MAN_PATH}/man3/lib-glem.3


manpages:
	@echo "Regenerating man pages from markdown"
	@ronn  -r doc/glem.ronn doc/lib-glem.ronn


obj/%.o: src/%.c
	@echo "Compiling $<"
	@$(CC) $(CC_FLAGS) -o "$@" -c "$<"


dirs:
	@mkdir -p obj lib inc


clean:
	@rm -rf obj/* lib/* glem


.PHONY: clean glem dirs install uninstall manpages


