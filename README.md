# Custom `cp` Implementation in C

A minimal reimplementation of the Unix `cp` command using low-level POSIX system calls (`open`, `read`, `write`, `stat`, `perror`, `fchmod`).

This project demonstrates how file copying works at the system-call level, including file descriptors, buffering and handling partial writes.

---

## Overview

This program:

- Accepts exactly two command-line arguments:
  - Source file
  - Destination file
- Opens the source in read-only mode
- Opens (or creates) the destination in write mode
- Copies data in 4KB chunks
- Handles partial writes correctly
- Stops copying at EOF

The implementation avoids standard I/O functions (`fopen`, `fread`, `fwrite`) and directly uses Unix system calls.

---

## System Calls Used

- `open()` — obtain file descriptors
- `read()` — read raw bytes from a file
- `write()` — write raw bytes to a file
- `close()` - close file descriptors
- `stat()` - get file information
- `fchmod()` - force apply source permission to destination
- `perror()` - print linux based error

Key concepts demonstrated:

- File descriptors
- Kernel-managed file offsets
- Buffered block copying (4096 bytes per read)
- Handling partial writes
- perror handling

---

## Build Instructions

Compile with:

```bash
gcc cp.c -o mycp
```

This produces the executable:

```
mycp
```

---

## Usage

```bash
./mycp <source_file> <destination_file>
```

Example:

```bash
./mycp input.txt output.txt
```

Behavior:

- If the destination file does not exist, it is created.
- If it exists, it is truncated (`O_TRUNC`).
- Default permissions for new files: `0644` (`rw-r--r--`).
- Source file permissions are applied to the destination file later.

---

## Implementation Details

### Buffer Size

```c
char buffer[4096];
```

- 4096 bytes (4KB)
- Matches typical Linux memory page size
- Reduces the number of system calls

---

### Copy Mechanism

The program uses a two-level loop:

1. **Outer loop** — Reads up to 4096 bytes from source.
2. **Inner loop** — Ensures all bytes read are fully written to destination (handles partial writes).

This ensures correctness even when `write()` writes fewer bytes than requested.

---

## Error Handling

The program validates:

- Correct number of arguments
- Source file open failure
- Destination file open failure

Errors result in a message and termination with a non-zero exit status.

---

## Current Limitations

- Does not handle copying into directories
- Does not preserve timestamps

---

## Possible Improvements

- Detect if destination is a directory
- Implement behavior similar to `cp -p`
- Use `sendfile()` for optimized zero-copy transfer

---

## Purpose

This project is educational and focuses on understanding:

- How Unix handles files internally
- How file descriptors map to kernel objects
- Why `ssize_t` is used for system call return values
- The importance of handling partial writes in systems programming

It is intentionally minimal and designed to reinforce low-level file I/O concepts.
