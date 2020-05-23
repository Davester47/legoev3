# legoev3 [EARLY-ALPHA]

This is a C library that provides access to routines available on the native ev3
firmware. It is in an EARLY-ALPHA stage, and its API is not yet stable. Library
functions can change without notice, as this is still in active development.

### To build

An arm-linux-gnueabi (or arm-none-linux-gnueabi) GCC toolchain is required to
build this library for the EV3. I don't reccommend this way anymore. Legoev3 
can be built with:
```
./configure --prefix=/path/to/install/directory --host=arm-linux-gnueabi
make
make install
```

Alternatively you use meson and ninja to build legoev3. You will have to build
legoev3 in a separate directory. I've provided a cross toolchain file for use
with meson. It's a little more complicated, but here's how to build it:
```
git clone https://github.com/Davester47/legoev3.git
mkdir build
cd build
meson ../legev3 --cross-file ../legoev3/cross.txt --prefix=/install/directory
ninja install
```

### To use legoev3 in a program.

Legoev3's functions are available by including `#include <ev3.h>`. It needs to
be initialized with `ev3Init();` and at the end of the program it needs to be freed with `ev3Free();`
Here's an example:

```C
#include <ev3.h>

int main() {
  ev3Init();
  //
  // Do some cool stuff in here. For more information, check out example/example.c
  //
  ev3Free();
}
```

To link against legoev3, use `-legoev3`. You will need to upload your program to the EV3 brick with a program like https://github.com/c4ev3/ev3duder.

### How it works:

Legoev3 doesn't actually contain any code that controls the motors. It provides
an interface to the kernel drivers that are built into the firmware. This library used to use the built-in libraries that interface to the drivers, but they way had too many dependencies. The libraries all needed each other, and the needed libusb which needed udev, and et cetera.

### To do list (not necessarily in this order):

- [x] Motors
- [x] Sensors
- [x] Buttons
- [x] LED Patterns
- [ ] Sound
- [ ] LCD
- [ ] Bluetooth
- [ ] WiFi
- [ ] A higher level API
- [ ] Real documentation :)
