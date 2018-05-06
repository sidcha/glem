/******************************************************************************

                  Copyright (c) 2017 Siddharth Chandrasekaran

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

    Author : Siddharth Chandrasekaran
    Email  : siddharth@embedjournal.com
    Date   : 07-May-2016

******************************************************************************/

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

