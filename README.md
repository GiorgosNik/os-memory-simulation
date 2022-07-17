# os-memory-simulation

## Description
The project aims to simulate two different memory management and replacement algorithms, specifically Least Recently Used (LRU) and Second Chance algorithms, based on the memory use of actual programs. The simulator executes the number of operations specified by its arguments, and after the simulation is complete, displays statistics that summarize the performance of the algorithm for the given input/argument combination.

## Instructions
The program includes a Makefile
To compile the simulation program: 
```
make all
```
To remove object files: 
```
make clean
```
To run the simulation: 
```
./sim -a -s -q -l 
```

The options for running the simulation are as follows:
- __Algorithm:__ This option specifies the page replacement algorithm to be used, `0` for LRU, `1` for Second Chance
- __Memory:__ This option specifies the number of frames of the main memory
- __Queue Size:__ This option specifies the pages read per loop from each trace file
- __Loop Amount:__ This option specifies the amount of loops to execute. This argument is optional with a default value of 10

## Implementation
As described above, the simulation takes as arguments the type of algorithm, the memory size, the traces read per loop and the number of loops. Given these, a Page Table is created, in the form of a hash table, in order to store information on the currently loaded pages, and if they involve a write operation or not. When a new trace is read from a trace file, the simulator tries to store it in the memory. If the memory is full, a previously stored page is removed based on the selected algorithm, and the new page is stored in its place. Each READ or WRITE operation is counted, as are the page faults that occur.

## Input and Output
The requests to use memory are simulated using 2 different trace data files, `bzip.trace` and `gcc.trace`. Each line of these two files contains information on one memory operation, in the form:
```
<MEMORY ADDRESS><OPERATION TYPE>
```
Where operation type is either `R` for read or `W` for write.

After the execution of the simulation, the following information is displayed:
- Input arguments of the simulation
- Total number of WRITE operations
- Total number of READ operations
- Number of page faults that ocured
- Total number of traces processed