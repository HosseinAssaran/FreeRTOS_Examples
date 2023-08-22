# FreeRTOS Examples
These examples originally are provided in the book [Mastering the FreeRTOS Real Time Kernel - a Hands On Tutorial Guide](https://www.freertos.org/fr-content-src/uploads/2018/07/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf) that you can find [here](https://www.freertos.org/Documentation/RTOS_book.html) with examples.
<p>They were only running on windows on Visual Studio. I've made them compatible with gcc and run all examples over fedora linux distrubition. You can now run them on every distribution of linux as well as windows.</p>
<p>I've used mps2-385 port FreeRTOS port and add requirments to compile it with gcc and run it over qemu emulator</p>
<p>Feel free to contribute to this project and all pull requests are wellcome.</p>

## GUIDENCE TO RUN Examples on Linux with GCC and using mps2-an385 port

<p>This part is dedicated to guide you how to run each example on linux. 
I've used xPack GNU Arm toolchain for compling code and xPack Qemu. </p>

### Toolchain Version
```
arm-none-eabi-gcc --version
arm-none-eabi-gcc (xPack GNU Arm Embedded GCC x86_64) 12.2.1 20221205
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
```

### QEMU version
```
qemu-system-arm --version
xPack QEMU emulator version 7.2.0 (v7.2.0-xpack)
Copyright (c) 2003-2022 Fabrice Bellard and the QEMU Project developers
```

### How to compile
1. `cd Examples/Example0**/GCC/`
2. Run `make`

### How to run simulator
`qemu-system-arm "--machine" "mps2-an385" "--cpu" "cortex-m3" "--kernel" "output/RTOSDemo.out" -monitor none -nographic -serial stdio`
