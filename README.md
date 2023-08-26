# FreeRTOS Examples
Welcome to the FreeRTOS example repository! These examples originally were provided in the book [Mastering the FreeRTOS Real Time Kernel - a Hands On Tutorial Guide](https://www.freertos.org/fr-content-src/uploads/2018/07/161204_Mastering_the_FreeRTOS_Real_Time_Kernel-A_Hands-On_Tutorial_Guide.pdf), which you can access [here](https://www.freertos.org/Documentation/RTOS_book.html) along with the accompanying examples.

## Introduction
<p>The provided examples were initially designed to run exclusively on Windows using Visual Studio. However, I've taken the initiative to enhance their compatibility by making them functional with the GCC compiler, enabling them to be executed on Fedora Linux as well as other Linux distributions and Windows systems.</p>

<p>To achieve this, I've utilized the mps2-385 port of FreeRTOS and introduced the necessary adjustments to facilitate compilation using GCC. Furthermore, I've ensured that the examples can be run on the QEMU emulator</p>

## Contributing
<p>I invite you to actively contribute to this project. Your contributions are highly valued, and pull requests are enthusiastically welcomed.</p>

## Running Examples on Linux with GCC using the mps2-an385 Port

<p>This section provides detailed instructions on how to run each example on a Linux environment. I've leveraged the xPack GNU Arm toolchain for compiling the code and the xPack QEMU emulator for execution. </p>

### Requirements 
---------------
<p>For simulating these examples over linux you only need toolchain and QEMU emulator.</p>

#### Toolchain
<p>To ensure compatibility, the following toolchain version was used:</p>

```
arm-none-eabi-gcc --version
arm-none-eabi-gcc (xPack GNU Arm Embedded GCC x86_64) 12.2.1 20221205
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
```
You can find instructions on how to install the toolchain [here](https://xpack.github.io/dev-tools/arm-none-eabi-gcc/install/).

#### QEMU 
<p>The chosen QEMU emulator version is as follows:</p>

```
qemu-system-arm --version
xPack QEMU emulator version 7.2.0 (v7.2.0-xpack)
Copyright (c) 2003-2022 Fabrice Bellard and the QEMU Project developers
```
You can find instructions on how to install QEMU for ARM [here](https://xpack.github.io/dev-tools/qemu-arm/install/).
### Run Examples
----------------
#### Compilation Steps
Follow these steps to compile the examples:
1. Navigate to the desired example's directory: `cd Examples/Example0**/GCC/`
2. Run the `make` command.

#### Running the Simulator
<p>Execute the following command to run the simulator for the example you compiled:</p>

`qemu-system-arm "--machine" "mps2-an385" "--cpu" "cortex-m3" "--kernel" "output/RTOSDemo.out" -monitor none -nographic -serial stdio`

<p>Feel free to explore and experiment with these examples. If you encounter any issues or have improvements to suggest, please contribute to the project or submit a pull request. Your involvement is pivotal in making this repository a valuable resource for the FreeRTOS community.</p>

## New Examples
<p> In this part, I explain about new exmpales I add to this project </p>

### Exmpale 026
<p> It has the same functionality of example 015. and try to keep back light on 
until recieved new key press within timeout.Polling method is used in 
the example 015 which increases cpu overhead.</p>
<p> In this example, we have used interrupt service routin for reading data from uart
instead of polling to decrease cpu overhead </p>