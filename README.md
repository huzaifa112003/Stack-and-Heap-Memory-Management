# Stack-and-Heap-Memory-Management

This project simulates stack and heap memory management using C structures and functions. It includes the creation and deletion of function frames, and the allocation of various data types within those frames. The project also handles the allocation and deallocation of memory buffers on the heap.

## Introduction

This project provides a simulation of stack and heap memory management by defining and manipulating data structures that represent memory frames, free list nodes, and allocated memory regions. The functions included allow for the creation, deletion, and management of memory within these structures.

## Structures

### framestatus

This structure represents the status of a frame in the stack. 

### freelist

This structure is used to manage free regions in memory.

### allocated

This structure keeps track of allocated memory regions.


## Algorithms and Functions

### create_frame

Creates a new frame in the stack for a given function.

### delete_function

Removes the most recently added function frame from the stack.

### create_integer

Creates an integer variable in the current frame on the stack.

### create_double

Creates a double variable in the current frame on the stack.

### create_char

Creates a char variable in the current frame on the stack.

### create_buffer

Creates a buffer on the heap.

### deal_buffer

Deallocates a buffer on the heap.

## Usage

To use this project, include the provided structures and functions in your C program. Call the functions as needed to simulate stack and heap memory management.

## Compilation

To compile the program, use the following command:

```sh
gcc -o memory_management memory_management.c
```
