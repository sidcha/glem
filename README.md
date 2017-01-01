# GLEM - Graphical LCD Emulator

A tiny (minimal) Graphical LCD emulator for speeding up UI development time for
embedded devices.

Often times, embedded developers realize UI enhancements only after seeing the
firmware in running on the device. So the task of building a good UI is very
time consuming. This project aims to eliminate the round trip time involved in
compiling, flashing, and checking the embedded device when working with GLCDs.
This is achieved by emulating a the GLCD in a window on your computer.

Dependencies
----------------
glem depends on the the following packages. 
	* freeglut3
	* freeglut3-dev
For example, if you are using a debian, install the packages with,
```sh
$ sudo apt-get install freeglut3 freeglut3-dev
```

Installation
----------------
Start by cloning the repository into your computer. You will need to have
git installed to do this.
```sh
$ git clone https://github.com/cbsiddharth/glem.git
$ cd glem
```
Compile the sources to produce glem binary with,
```sh
$ make
$ make install  # (optional)
```

How to use
----------------
Data Encoding:
Most commercially available GLCDs are 1 bit per pixel packed into bytes in two
different ways.
1. Row Major - MSB of first byte is pixel (0,0) and LSB is pixel (7,0). Second
   byte represents pixels (8,0) to (15,0) and so on.
2. Column Major - MSB of first byte is pixel (0,0) and LSB is pixel (0,7).
   Second byte represents pixels (1,0) to (1,7) and so on.

In both cases, typical application layer code has an array to store the bit map
value. The length of the array would be,
```
buf_len = glcd_width/8 * glcd_height;
```
This buffer is what your embedded device writes out to the GCLD through the
SPI/I2C bus. The glem server expects to receive this buffer to render the image.

In your application you will have switch low level SPI/I2C driver calls with
some mock functions provided here. Then compile your GLCD layer on GCC and run
it to see the rendered display. Once all you testing has been completed, you
can switch back to writing data to SPI/I2C.

Start the server with the glcd's width and heigth in pixels. 

```sh
$ glem -r 128x63
```
Once the server has been launched (you should see a new window), it sets up a
UNIX domain socket at /tmp/. Your application can connect to this socket and
write the bitmap array data to be displayed. 

You can test the server with the following command.
```sh
$ glem -t -r 128x64
```
Have a look at src/main.c to get and idea on how to use it in your project.

### Bugs and Patches.

The git upstream for this project is hosted at http://github.com/cbsiddharth/glem.git
You can report bugs to siddharth@embedjournal.com.

Patches are welcome.

