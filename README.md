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
Start the emulation server. Once the server has been launched, it sets up a UNIX domain socket at /tmp/glcdServer. Your application can connect to this socket and write the bitmap to be displayed. The data has to be encoded in row major format (each bit represents a pixel on the GLCD) and sent to the server. You also need to have socat installed for the following to work. 
```sh
$ perl -e '$t=((shift)/8)*(shift); print "\x55"x$t;' 240 128 | socat UNIX-CONNECT:/tmp/glcdSocket -
```
