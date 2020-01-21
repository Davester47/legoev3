# legoev3 [EARLY-ALPHA]

This is a C library that provides access to routines available on the native ev3
firmware. It is in an EARLY-ALPHA stage, and its API is not yet stable. Library
functions can change without notice, as this is still in active development.

### To build

An arm-linux-gnueabi (or arm-none-linux-gnueabi) GCC toolchain is required to
build this library for the EV3. Legoev3 can be built with:
```
./configure --prefix=/path/to/arm/toolchain --host=arm-linux-gnueabi
make
make install
```

### To use legoev3 in a program.

Legoev3's functions are available by including `#include <ev3.h>`. It needs to
be initialized with `ev3Init();` and at the end of the program it needs to be freed
with `ev3Free();`
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

To link against legoev3, use `-lc_output -legoev3`. The linker
flag `-Wl,-rpath,/home/root/lms2012/sys/lib` is required to use legoev3. You will
need to upload it to the EV3 with a program like https://github.com/c4ev3/ev3duder.

### How it works:

Legoev3 doesn't actually contain any code that controls the motors. It provides
an interface to shared C libraries that are built into the firmware. The shared
libraries are located at /home/root/lms2012/sys/lib, and the -rpath linker flag
tells the program where to find them.

### Coming Soon:

Input and sensor support is in the works.
