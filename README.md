# OS2

## Instructions
The program includes a Makefile
- To compile the simulation program: `make all`
- To remove object files: `make clean`
- To run the simulation: `./sim -a <Algorithm> -s <Memory> -q <Queue Size> -l <Loop Ammount>`

The options for running the simulation are as follows:
- __Algorithm:__ This option specifies the page replacement algorithm to be used, `0` for LRU, `1` for Second Chance
- __Memory:__ This option specifies the number of frames of the main memory
- __Queue Size:__ This option specifies the pages read per loop
- __Loop Ammount:__ This option specifies the ammount of loops to execute