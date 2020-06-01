# legoev3 [EARLY-ALPHA]

This is a C library that provides access to routines available on the native ev3 firmware. It is in an EARLY-ALPHA stage, and its API is not yet stable. Library functions can (and probably will) change without notice, as this is still in active development.

### To build

Meson, ninja, and an arm-linux-gnueabi gcc cross compiler are required to build legoev3. On Debian (probably Ubuntu too), they can be installed with `sudo apt install meson ninja-build crossbuild-essential-armel`. If you insist on using Windows, you'll need to find a suitable cross compiler.

```
git clone https://github.com/Davester47/legoev3.git
mkdir build
cd build
meson ../legoev3 --cross-file ../legoev3/cross.txt --prefix=/install/directory
ninja install
```

### To use legoev3 in a program.

Legoev3's functions are available by including `#include <ev3.h>`. It needs to be initialized with `ev3Init();` and at the end of the program it needs to be freed with `ev3Free();`
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

To link against legoev3, use `-legoev3`. You will need to upload your program to the EV3 brick. I recommend you use a program like https://github.com/c4ev3/ev3duder.

### How it works:

Legoev3 doesn't actually contain any code that controls the motors. It provides an interface to the kernel drivers that are built into the firmware. This library used to use the built-in libraries that interface to the drivers, but they way had too many dependencies. The libraries all needed each other, and the needed libusb which needed udev, and et cetera.

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
- [ ] Unit Tests
- [ ] Real documentation :)
