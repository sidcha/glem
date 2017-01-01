/***************************************************************************
 * This program is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                         *
 **************************************************************************/

/*
 *        File: glem/src/glem-client.c
 *  Created on: 07-May-2016
 *      Author: Siddharth Chandrasekaran
 *        Mail: siddharth@embedjournal.com
 */

#include <stdio.h>
#include <stdlib.h>

#include "symbols.h"
#include "fonts.h"
#include "objects.h"
#include "glcd.h"

int main(int argc, char *argv[])
{
	int w, h, lw, lh;
	if (argc != 3) {
		printf("Usage: %s <width> <height>\n", argv[0]);
		exit(-1);
	}
	w = atoi(argv[1]);
	h = atoi(argv[2]);
	glcd_init(w, h, GLCD_ROW_MAJOR);

	probe_symbol(&glem_logo, &lw, &lh);
	draw_symbol(&glem_logo, (w-lw)/2, (h-lh)/2);

	glcd_refresh();
	return 0;
}

