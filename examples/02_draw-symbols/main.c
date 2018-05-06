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

// local files in the same directory.
// the actual glem.h is included in glcd.c.
#include "symbols.h"
#include "fonts.h"
#include "objects.h"
#include "glcd.h"

int main(int argc, char *argv[])
{
	int w, h, logo_width, logo_height;
	if (argc != 3) {
		printf("Usage: %s <width> <height>\n", argv[0]);
		exit(-1);
	}
	w = atoi(argv[1]);
	h = atoi(argv[2]);

	if (w < 128 || h < 64) {
		// the image we are about to write cannot fit
		// nicely in resolutions less than 128x64.
		printf("Error: resolution has to be 128x64 or greater.\n");
		exit(1);
	}

	// init GLCD and glem layer.
	glcd_init(w, h, GLCD_ROW_MAJOR);

	// get the symbols widht and height.
	probe_symbol(&glem_logo, &logo_width, &logo_height);

	// center the logo on awailable space and then draw.
	draw_symbol(&glem_logo, (w-logo_width)/2, (h-logo_height)/2);

	// calls glem_write with the complete frame buffer.
	glcd_refresh();
	
	// you can also set pixels directly!
	glcd_set_pixel_direct(2,2, 1);

	return 0;
}

