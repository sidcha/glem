# glem - Graphical LCD Emulator
A tiny (minimal) Graphical LCD emulator for speeding up UI developement of embedded devics.

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
### Usage
Start the server with the glcd's width and heigth in pixels. 
```sh
$ ./glem serve 128 64
```

Once the server has been launched (you should see a new window), it sets up a UNIX domain socket
at /tmp/. Your application can connect to this socket and write the bitmap (row major format) to
be displayed. 

*Note:* data has to be encoded in row major format: each bit represents a pixel on the GLCD with
MSB of first byte at pixel (0,0) and LSB of first byte at pixel (0,7) and so on.

You can test the server with the following commands, 
```sh
$ perl -e '$t=((shift)/8)*(shift); print "\x55"x$t;' 128 64 |\
		socat UNIX-CONNECT:/tmp/glcdSocket128x64 -
$ ./glem test
```

You can report bugs to siddharth3141@gmail.com. Patches are welcome.

