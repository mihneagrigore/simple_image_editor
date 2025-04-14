# Marching Squares Algorithm Documentation

This document describes the implementation of the **Marching Squares** algorithm, outlining its purpose, functions, and the specific operations performed on image data.

## Table of Contents
1. [Overview](#overview)
2. [Functions Overview](#functions-overview)
    - [main](#main)
    - [resize](#resize)
    - [grid](#grid)
    - [tresholding](#tresholding)
    - [bits](#bits)
    - [read_commands](#read_commands)
3. [Conclusion](#conclusion)

## Overview

The implemented algorithm follows the problem's specification by reading image data, including pixels and commands (given as strings). The algorithm is modularized into distinct functions for memory allocation, deallocation, reading data, grid creation, and applying the **Marching Squares** algorithm. Dynamic memory allocation is used for matrices due to their large dimensions, with the exception of the contour matrix, which is declared on the stack (16x16 size).

The `main` function handles edge cases (such as `EXIT` and `READ` commands) and manages error messages. The rest of the commands are processed in the `read_commands` function, which also initializes the contour matrix with appropriate values. Each command is handled individually, allocating and deallocating memory as needed.

The pixel matrix is only deallocated once all operations on it have been completed.

## Functions Overview

### main

The `main` function is responsible for handling edge cases, such as `EXIT` and `READ` commands, and displaying corresponding error messages. It also invokes the `read_commands` function to process all other commands. The function ensures that memory is properly allocated and deallocated, and handles any necessary cleanup at the end of the program's execution.

### resize

The `resize` function transforms the pixel matrix according to the specification, making it four times larger by multiplying the existing elements. This resizing operation expands the matrix to accommodate the necessary image size for further processing.

### grid

The `grid` function constructs the grid as specified in the problem statement. It treats edge cases separately, such as elements that don't have four neighbors, as well as the extra row and column that may be required. The grid is created dynamically to represent the image structure needed for **Marching Squares** processing.

### tresholding

The `tresholding` function modifies the grid based on a reference value (200). This step is crucial for filtering the grid elements and ensuring that only the relevant parts of the grid are processed by the **Marching Squares** algorithm.

### bits

The `bits` function performs the final step of the **Marching Squares** algorithm. It calculates the value of each square in the grid using base 10 and copies the corresponding contour. This step is essential for determining the final result of the **Marching Squares** algorithm.

### read_commands

The `read_commands` function interprets the various commands given by the user, initializing the contour matrix and performing necessary memory allocations. It processes each command individually and ensures that memory is freed once the command has been executed. This function is integral to the entire process, as it handles all commands except for the `EXIT` and `READ` commands, which are handled by the `main` function.

## Conclusion

The **Marching Squares** algorithm is designed to process image data based on user-provided commands. It allocates memory dynamically for the pixel matrix and grid, handling edge cases and performing image resizing, grid creation, thresholding, and the core **Marching Squares** algorithm. Memory is properly managed, with deallocation occurring only once all commands are completed. The modular structure of the program ensures that each function is responsible for a specific task, making the code more readable and maintainable.
