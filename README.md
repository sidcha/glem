**GLEM** - Graphical LCD Emulator
=================================

GLEM is a tiny (minimal) graphical LCD emulator for embedded platforms and other
small projects that need a one-off GUI to display some data without having to
write too much GUI Code.

Often, embedded developers realize UI enhancements only after seeing the
firmware in running on the device. So the task of building a good UI is very
time consuming. 

This project aims to eliminate the round trip time involved in compiling,
flashing, and checking the embedded device when working with GLCDs.  This is
achieved by emulating a the GLCD in a window on your computer.

Dependencies
------------
GLEM uses OpenGL and FreeGLUT3. On linux you will have to install the following
packages to build GLEM.

* freeglut3
* freeglut3-dev

On windows, you can install MinGW GCC and FreeGLUT3 from [here][1].

Installation
------------

Clone the repository and switch to the cloned directory. Compile the sources
to produce glem binary with,

```sh
$ make
$ sudo make install  # (optional)
```

Description
-----------

**Data Encoding:**

Most commercially available GLCDs are 1 bit per pixel packed into bytes in two
different ways.
1. Row Major - MSB of first byte is pixel (0,0) and LSB is pixel (7,0). Second
   byte represents pixels (8,0) to (15,0) and so on.
2. Column Major - MSB of first byte is pixel (0,0) and LSB is pixel (0,7).
   Second byte represents pixels (1,0) to (1,7) and so on.

*Note:* Here, assume that origin (0,0) is at top left corner of the GLCD. 

In both cases, typical application layer code has an array (back-planes) to
store the bitmap value. The length of the array would be,
    
```c
buf_len = (glcd_width*glcd_height)/8
```

This buffer is what your embedded device writes out to the GLCD through the
SPI/I2C bus. The glem server expects to receive this buffer to render the
image.

**GLEM development**

glem-dev provies the following functions through the header glem.h. If you did
`sudo make install` in the previous step, glem.h should be available in standard
path.
```c
#include <glem.h>

void glem_init(int glcd_width, int glcd_height, int flags);
void glem_write(uint8_t *buf, int len);
void glem_setpix(int x, int y, int color);
```
In your application you will have to include glem.h in your project and
replace low level SPI/I2C driver calls to `glcd_write` function with the 
`glem_write` function. Then compile your GLCD layer on GCC and link it with 
`-lglem` to get it to send data to an already running glem server. 

How to use
----------------
Start the server with the your GLCD's resolution in pixels. Optionally, you could
scale the GLCD display so it looks good on your computers screen. In the
following example, glem is scaled 4 times.

```sh
$ glem -r 128x63 -s4
```
Once the server has been launched (you should see a new window), it sets up a
TCP server for your application to connect and write bitmaps or individual bits.
See `examples/` for more details.

Have a look at the example dir to get an idea of how your application should
be structured.

### Bugs and Patches.

The git upstream for this project is hosted at [github][2], you can raise a PR
(prefered) or you can send the patch to my mail and I will commit on your
behalf.

To report bugs/issues, you can create an issue in github project page (prefered)
or you can write an email to me with the description.

*Author:* Siddharth Chandrasekaran
*Email:* siddharth@embedjournal.com

[1]: http://www.transmissionzero.co.uk/software/freeglut-devel/
[2]: http://github.com/cbsiddharth/glem.git
