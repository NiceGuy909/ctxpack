# ctxpack

A C++ CLI tool for scanning a repository and extracting useful file-level
metadata and source content, with the eventual goal of generating compact,
LLM-friendly repository context.

## Current Status

Early development — Day 1.

Currently, `ctxpack` can:

- Recursively scan a repository
- Skip generated/dependency directories:
  - `.git`
  - `node_modules`
  - `dist`
  - `build`
- Collect file paths
- Detect file extensions
- Report file sizes
- Heuristically distinguish text and binary files
- Count lines in text files
- Read text-file contents
- Handle invalid repository paths and inaccessible files

## Build

Requirements:

- C++20
- CMake

```bash
cmake -S . -B build
cmake --build build

The executable will be created at:

```text
build/ctxpack
```

## Usage

Pass the path to a repository or directory as the first argument:

```bash
./build/ctxpack <path>
```

For example:

```bash
./build/ctxpack "$HOME/Desktop/my-project"
```

You can also use a relative path:

```bash
./build/ctxpack ./my-project
```

## Example Output

```text
Path: "/home/user/project/src/main.cpp"
Extension: ".cpp"
Size: 1542 Bytes
Line Count: 48
text/binary: 1

Path: "/home/user/project/assets/logo.png"
Extension: ".png"
Size: 13057 Bytes
text/binary: 0

==== "/home/user/project/src/main.cpp" ====
#include <iostream>

int main() {
    std::cout << "Hello World\n";
}

Files Found: 2
```

`text/binary: 1` means the file was detected as text.

`text/binary: 0` means the file was detected as binary.
