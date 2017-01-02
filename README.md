GLEM - **G**raphical **L**CD **Em**ulator
=============================

glem is a tiny (minimal) graphical LCD emulator for embedded platforms and other
small projects that need a one-off GUI to display some data without having to
write too much GUI Code.

Often, embedded developers realize UI enhancements only after seeing the
firmware in running on the device. So the task of building a good UI is very
time consuming. This project aims to eliminate the round trip time involved in
compiling, flashing, and checking the embedded device when working with GLCDs.
This is achieved by emulating a the GLCD in a window on your computer.

Dependencies
----------------
glem depends on the the following packages. 
* freeglut3
* freeglut3-dev
You will also need the following packages. Ignore if you already have them.
* git

So you will have to install them to compile glem. Copy paste the following if
relevant,
```sh
$ sudo apt-get install freeglut3 freeglut3-dev git
```

Installation
----------------
Start by cloning the repository into your computer.
```sh
$ git clone https://github.com/cbsiddharth/glem.git
$ cd glem
```
Compile the sources to produce glem binary with,
```sh
$ make
$ sudo make install  # (optional)
```

**Data Encoding:**
Most commercially available GLCDs are 1 bit per pixel packed into bytes in two
different ways.
1. Row Major - MSB of first byte is pixel (0,0) and LSB is pixel (7,0). Second
   byte represents pixels (8,0) to (15,0) and so on.
2. Column Major - MSB of first byte is pixel (0,0) and LSB is pixel (0,7).
   Second byte represents pixels (1,0) to (1,7) and so on.

In both cases, typical application layer code has an array to store the bit map
value. The length of the array would be, `buf_len = glcd_width/8 *
glcd_height;`

This buffer is what your embedded device writes out to the GLCD through the
SPI/I2C bus. The glem server expects to receive this buffer to render the
image.

glem-dev provies the following functions through the header glem.h. If you did
`sudo make install` in the previous step, glem.h should be available in standard
path
```c
#include <glem.h>

void glem_init(int glcd_width, int glcd_height, int flags);
void glem_server_send(uint8_t *buf, int len);
```
In your application you will have to include glem.h in your project  and
replace low level SPI/I2C driver call with the glem_server_send function. Then
compile your GLCD layer on GCC and link it with -lglem to get it to send data
to an already running glem server. Once all you testing has been completed, you
can switch back to writing data to SPI/I2C.

How to use
----------------
Start the server with the your GLCD's resolution in pixels. Optionally, you could
scale the GLCD display so it looks good on your computers screen. In the
following example, glem is scaled 4 times.
```sh
$ glem -r 128x63 -s4
```
Once the server has been launched (you should see a new window), it sets up a
UNIX domain socket at /tmp/. Your application can connect to this socket and
write the bitmap array data to be displayed. 

Once the server is launched, you can test the server (same resolution) with 
some fixed data to see if you are able to write something.
```sh
$ glem -r 128x64 -t 85  # some lines
$ glem -r 128x64 -t 0   # clear the display
$ glem -r 128x64 -t 255 # set all pixels
```
Have a look at the example dir to get an idea of how your application should
be structured.

### Bugs and Patches.

The git upstream for this project is hosted at [github](http://github.com/cbsiddharth/glem.git).
You can report bugs to siddharth@embedjournal.com or create an issue there.
This project is in active development, patches are welcome.

