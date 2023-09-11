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

## Book Examples
<p> In this part, I explain about what each book example does.</p>

### Exmpale 01
<p> Creating tasks. Running two task at the same priority level having diffrent task functions.</p>

### Exmpale 02
<p> Using the task parameter. Running two task at the same priority level sharing the same task function.</p>

### Exmpale 03
<p> Experimenting with priorities.</p>

### Exmpale 04
<p> Using the Blocked state to create a delay.</p>

### Exmpale 05
<p> Converting the example tasks to use vTaskDelayUntil().</p>

### Exmpale 06
<p> Combining blocking and non-blocking tasks.</p>

### Exmpale 07
<p> Defining an idle task hook function.</p>

### Exmpale 08
<p> Changing task priorities.</p>

### Exmpale 09
<p> Deleting tasks.</p>

### Exmpale 10
<p> Blocking when receiving from a queue.</p>

### Exmpale 11
<p> Blocking when sending to a queue, and sending structures on a queue.</p>

### Exmpale 12
<p> Using a Queue Set.</p>

### Exmpale 13
<p> Creating one-shot and auto-reload timers.</p>

### Exmpale 14
<p> Using the callback function parameter and the software timer ID.</p>

### Exmpale 15
<p> Resetting a software timer.</p>

### Exmpale 16
<p> Using a binary semaphore to synchronize a task with an interrupt.</p>

### Exmpale 17
<p> Using a counting semaphore to synchronize a task with an interrupt.</p>

### Exmpale 18
<p> Centralized deferred interrupt processing.</p>

### Exmpale 19
<p> Sending and receiving on a queue from within an interrupt.</p>

### Exmpale 20
<p> Rewriting vPrintString() to use a semaphore.</p>

### Exmpale 21
<p> Re-writing vPrintString() to use a gatekeeper task.</p>

### Exmpale 22
<p> Experimenting with event groups.</p>

### Exmpale 23
<p> Synchronizing tasks.</p>

### Exmpale 24
<p> Using a task notification in place of a semaphore, method 1.</p>

### Exmpale 25
<p> Using a task notification in place of a semaphore, method 2.</p>

----------------

## New Examples
<p> In this part, I explain about new exmpales I add to this project </p>

### Exmpale 026
<p> It has the same functionality of example 015. and try to keep back light on 
until recieved new key press within timeout.Polling method is used in 
the example 015 which increases cpu overhead.</p>
<p> In this example, we have used interrupt service routin for reading data from uart
instead of polling to decrease cpu overhead.</p>

### Exmpale 027
<p> It echos each character you send to uart0 using interrupt and queue.</p>

### Exmpale 028
<p> Each time, it waits for 16 characters to be typed with 10 seconds timeout.
It uses buffer instead of queue.</p>

### Exmpale 029
<p> Each time, it waits for stream buffer filled with at least 10 characters 
with 10 seconds timeout. It shows using stream buffer which is filled with interrupt.</p>

### Exmpale 030
<p> Using CMSIS-RTOS as a wrapper for running FreeRTOS.</p>

### Exmpale 031
<p> Using FreeRTOS-POSIX as a wrapper for running FreeRTOS.</p>

### Exmpale 032
<p> Using heap 1 for memory management and try to delete task created by heap.</p>

### Exmpale 033
<p> Using heap 1 for memory management and try to delete task created statically.</p>

### Exmpale 034
<p> Using heap 1 for memory management and try to build a queue statically.</p>

### Exmpale 035
<p> Developing an abstract layer by using an LED interface and initilize leds 
to turn on and off repeatedly.</p>

You can modify and add what implementaion you want in `ledImplementation.c`

### Exmpale 036
<p> Get data from uart and actuate it on LEDs. It shows abstraction and loosly coupled code. 
I can suceeded to see the result by the simulator.</p>

You can modify and add what implementaion you want in `pwmImplementation.c`



