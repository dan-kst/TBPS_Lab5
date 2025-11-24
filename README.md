# Linear Equation Solver (Gaussian Elimination)

This project implements the Gaussian Elimination method to solve systems of linear algebraic equations. It is designed to demonstrate and compare the performance of different parallel computing techniques against a sequential baseline.


## Features

 - Core Algorithm: Gaussian elimination with partial pivoting for numerical stability.
 - Sequential Implementation: Standard single-threaded execution.
 - OpenMP Implementation: Parallelization using compiler directives.
 - Performance Analysis: Automatic timing of calculation phases (excluding I/O) to measure speedups.
 - File I/O: Support for saving/loading matrices and solutions from text files.


## Prerequisites

To build this project, you need a C++ compiler supporting C++17, **Meson** and **Ninja**. You also need the OpenMP library.


### 🐧 GNU/Linux (Debian/Ubuntu)

```bash
sudo apt install build-essential meson ninja-build libomp-dev
```


### 🪟 Windows

1. **Compiler**: Install [Visual Studio Community](https://visualstudio.microsoft.com/) (select "Desktop development with C++" during installation) OR [MinGW-w64](https://www.mingw-w64.org/).
2. **Build System**: Install Python, then run in Shell:
```shell
pip install meson ninja
```
3. **OpenMP**: Usually included with Visual Studio (MSVC) and MinGW. No extra steps required.


### 🍎 MacOS

1. **Compiler**: Install Xcode Command Line Tools:
```bash
xcode-select --install
```
2. **Dependencies**: Install [Homebrew](https://brew.sh/), then run:
```bash
brew install meson ninja libomp
```


## Building the Project

This project uses Meson. Follow these steps to compile:

1. Clone the repository:
```git
git clone https://github.com/dan-kst/TBPS_Lab6.git
cd TBPS_Lab6
```
2. Setup the build directory:
```bash
meson setup build
```
*Note: If you encounter errors regarding OpenMP on MacOS, you may need to point Meson to the brew installation or ensure libomp is linked.*

3. Compile:
```bash
meson compile -C build
```
or
```bash
ninja -C build
```
*Note: `-C` means "enter directory before run the command" (in our case `build`). Also `meson compile` will call Ninja to compile, so using `ninja` is a little faster and convenient.*


## Usage

After compiling, the executable will be located in the `build` directory.

### Running the Program

#### Linux / MacOS
./build/lab6_program

#### Windows
.\build\lab6_program.exe

## Input File Format

If loading a matrix from a file, use the following plain text format:
1. The first line contains the size of the system N.
2. The following N lines contain the augmented matrix (coefficients + result vector).

```Plaintext
2
2.0 3.0 8.0
1.0 -1.0 -1.0
```
*(Represents: 2x + 3y = 8, 1x - 1y = -1)*

## Performance Comparison

The program runs several tests to compare execution times:
1. Sequential: Runs on a single core.
2. OpenMP: Uses #pragma omp parallel for to distribute row elimination across cores.

*Note: For small matrices (N < 500), the overhead of creating threads may make the parallel versions slower. Performance gains are most visible with N > 1000.*

## Project Structure

- **main.cpp**: Entry point, runs tests and timing benchmarks.
- **GaussSolver.cpp**: The core Matrix class containing the solver logic and specific solver implementation.
- **meson.build**: Build configuration definitions.
