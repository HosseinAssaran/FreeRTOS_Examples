# CheatSheet For Debug
## In Termina 1:
```
cd FreeRTOS_Examples/Examples/Example001/GCC/
make
qemu-system-arm "--machine" "mps2-an385" "--cpu" "cortex-m3" "--kernel" "output/RTOSDemo.out" -monitor none -nographic -serial stdio -S -s
```
## In Terminal 2:
```
cd FreeRTOS_Examples/Examples/Example001/GCC/
vim ../main.c
:set number
:packadd termdebug
:let g:termdebugger = "gdb-multiarch"
:Termdebug
Ctrl+w Ctrl+w, Ctrl+w Ctr+w, Ctrl+w L, Ctrl+w Ctrl+w
file output/RTOSDemo.out
break 114
target remote localhost:1234
continue
```
