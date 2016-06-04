# GLEM - Graphical LCD Emulator

A tiny (minimal) Graphical LCD emulator for speeding up UI development time for  embedded devices.

The purpose of this project is to eliminate the round trip time involved in flashing
the embedded device when working with GLCDs. This is achieved by emulating a the GLCD in a window
on your computer.

In your application you will have switch low level SPI/I2C driver calls with some mock functions
provided here. Then compile your GLCD layer on GCC and run it to see the rendered display. Once
all you testing has been completed, you can switch back to writing data to SPI/I2C.

### Data Encoding

Most commercially available GLCDs are 1 bit per pixel packed into bytes in two different ways.
1. Row Major - MSB of first byte is pixel (0,0) and LSB is pixel (7,0). Second byte represents pixels
(8,0) to (15,0) and so on.
2. Column Major - MSB of first byte is pixel (0,0) and LSB is pixel (0,7). Second byte represents
pixels (1,0) to (1,7) and so on.


Therefore, the glcd buffer length would be,
```
buf_len = glcd_width/8 * glcd_height;
```

This buffer is what your embedded device sends out to the GCLD through the SPI/I2C bus and the glem server
expects same buffer to render the image.

### Dependencies
* freeglut3
* freeglut3-dev
```sh
$ sudo apt-get install freeglut3 freeglut3-dev
```

### Installation
```sh
$ git clone https://github.com/cbsiddharth/glem.git
$ cd glem
$ make
```

### How to use


Start the server with the glcd's width and heigth in pixels. 

```sh
$ ./glem serve 128 64
```

Once the server has been launched (you should see a new window), it sets up a UNIX domain socket
at /tmp/. Your application can connect to this socket and write the bitmap (row major format) to
be displayed. 

*Note:* data has to be encoded in row major format.

You can test the server with the following command.
```
$ ./glem test 128 64
```

Have a look at src/example.c to get and idea on how to use it in your project.

### Bugs and Patches.

The git upstream for this project is hosted at http://github.com/cbsiddharth/glem.git
You can report bugs to siddharth3141@gmail.com. Patches are welcome.
