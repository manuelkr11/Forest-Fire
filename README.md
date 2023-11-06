# Forest fire simulation using C++ and SDL2

## Introduction

The goal of this project was the implmentation of a simulation of a forest fire using C++ and SDL2 for rendering the output. The simulation shall include a cellular automaton. The complete specification of the task can be found in `docs/task.pdf`.

## Compiling

A makefile was added for easier compilation of the main program. To compile all the files simply go to the main directory and type
```console
make
```

If linkage errors appear during this process you should make sure the provided SDL library is right for your system and C++ compiler (Windows 10 with a 32bit compiler). If not, download the SDL2 library from [here](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.4) and copy both the library `SDL2.dll` itself as well as the folders `src/include` and `src/lib` of the right version into this project. This [tutorial](https://www.youtube.com/watch?v=H08t6gD1Y1E&t=85s) might help in the process.

## Usage

### Starting and Flags

To start the simulation with all default setting simply type
```console
./main
```

For calculating the simulation time (and therefore diabeling the graphics output) use the following flag.
```console
./main --measure
```

Also, the number of generations (when measuring the calculation time), the height and width of the forest and the number of threads (for multithreading) can be changed.
```console
./main --height 1024 --width 1024 --measure --generation 1000 --threads 8
```

### Features

There are some implemented features as follows:

|Key   |Function   |
|---|---|
|j/k   |Decrease/Increase p_fire   |
|n/m   |Decrease/Increase p_tree   |
|left mouse   |Start Fire   |
|Esc   |Close   |

### Results for the orbital TODO

After closing the simulation (when in measuring mode), the calculation times will be stored as `results.txt`.

## Solutions

The requested document containing a short description of the code, the results and some discussion point are found as `docs/results.pdf`. A short video of the running programm has also been recorded and is saved as `docs/program.mp4`.
