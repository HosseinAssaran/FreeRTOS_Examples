** Toolchain Version
```
arm-none-eabi-gcc --version
arm-none-eabi-gcc (xPack GNU Arm Embedded GCC x86_64) 12.2.1 20221205
Copyright (C) 2022 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
```

** How to compile
1. `cd Examples/Example0**/GCC/`
2. run `make`

** How to run simulator
`qemu-system-arm "--machine" "mps2-an385" "--cpu" "cortex-m3" "--kernel" "output/RTOSDemo.out" -serial stdio -monitor none -nographic`
