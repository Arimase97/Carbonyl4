# Carbonyl4: A Sketch for Set-Increment Mixed Updates

This repository contains the source code and supporting files for the paper "Carbonyl4: A Sketch for Set-Increment Mixed Updates." Follow the instructions below to compile and run the code, and to generate figures.

## Compilation

To compile the code, use the following command:

```
g++ main.c murmur3.c -o main -O2 -std=c++17
```

This command will compile the `main.c` and `murmur3.c` files using the `g++` compiler with optimization level 2 and the C++17 standard.

## Running the Code

After compiling the code, you can run it using:

```
./main > figure/raw_data.txt
```

This command will execute the compiled program and redirect its output to `figure/raw_data.txt`.

## Generating Figures

To generate the figures, run:

```
python3 figure.py
```

This script will process the data and generate the figures stored in the `figure` folder.

## Additional Figures

Note that the `figure` folder contains more images than those presented in the paper. These additional figures were not included in the paper due to space constraints but are available here for further reference and analysis.
