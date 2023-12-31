# Ghost Simulator
## Description
A multi-threaded ghost hunting simulation written in C. 
With the house layout based on the popular steam game [Phasmaphobia](https://store.steampowered.com/app/739630/Phasmophobia/), 
this program allows you to simulate hunting a ghost in a 4v1 situation
with an aggressive ghost that wants to ensure every hunter becomes too
scared to complete the game. The hunters win by finding enough evidence to figure out
what type of ghost is in the house, if they are unable to do this the ghost will win.
All actions made by both the hunters and the ghost are printed to the console so the user 
can keep track of what is happening in the simulation as it runs, or review the data 
from the text output after the fact to review how the hunt went.

## Compilation
A Make file is included in this program making it compile with a single command and easy to quickly
clean up the Object files created during compilation using a clean task in the Make file.

Compiling the program can be done in one command:
```shell
make 
```
\
Then it can be cleaned using:
```shell 
make clean
```

## Usage
To give the hunters random equipment:
```shell
./ghost_simulator random_start
```
\
For user chosen hunter equipment: 
```shell
./ghost_simulator
```

## Technical Details
Since this program is multi-threaded there were some thread synchronization issues that needed to be addressed.
Data shared between threads was locked behind a semaphore to ensure only one thread could read or write to the data 
at any given time. Although this fixes the issues with race conditions some actions required two 
pieces of shared data which introduced deadlocks. These deadlocks were solved using a helper function
that would ensure the thread that was created earlier would always be locked first. 
Since printing to the console is not a thread safe process there are sometimes issues where 
the text buffer would be overwritten by another thread before the printing is completed. 
While this could be combated with more semaphores. It would also greatly slow down the program since every action is logged, 
it would become essentially single threaded. This will not be a problem 
as it also logs the actions to a file which is reliable enough 
to be used as a detailed log of actions performed during the simulation.

## Contributors
https://github.com/HersheyDK
