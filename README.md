# glem - Graphical LCD Emulator
A tiny (minimal) Graphical LCD emulator for speeding up UI developement time for embedded devics.

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
$ sudo make install # (optional)
```
### Usage
Start the emulation server by,
```sh
$ ./glem # from cloned directory. (or)
$ glem # from any dorectory if you did [sudo make install] in last step.
```
Once the server has been launced, it sets up a UNIX domain socket at /tmp/glcdServer. Your application can connect to this socket and write the bitmap to be displayed. The data has to be encoded in row major format (each bit represents a pixel on the GLCD) and sent to the server.

### Example
you need to have socat installed for this to work. 
```sh
$ perl -e '$t=((shift)/8)*(shift); print "\x55"x$t;' 240 128 | socat UNIX-CONNECT:/tmp/glcdSocket -
```
