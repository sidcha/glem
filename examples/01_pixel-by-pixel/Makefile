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
#  Created on: 01-Jan-2017
#      Author: Siddharth Chandrasekaran
#        Mail: siddharth@embedjournal.com
#

ROOT_DIR := ../..

all: pixel-by-pixel

pixel-by-pixel:
	@echo "Building example $@"
	@gcc -Wall -O3 -o $@ main.c -lglem -L$(ROOT_DIR)/lib -I$(ROOT_DIR)/include

clean:
	@rm -rf pixel-by-pixel *.o

.PHONY: clean dirs pixel-by-pixel

