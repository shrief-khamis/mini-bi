# miniBI

`miniBI` is a lightweight desktop business intelligence (BI) application built with Qt.

The goal is to provide a fast, approachable way for users to create simple plots and explore data without relying on heavy, multi-GB analytics tools. The project is intentionally evolving, so architecture and features will be refined iteratively.


## Prerequisites

Before building, ensure these dependencies are installed and available in your environment:

- CMake 3.16 or newer
- Qt development packages

If CMake cannot locate Qt automatically, set either:

- `CMAKE_PREFIX_PATH` to your Qt installation prefix, or
- `Qt6_DIR` / `Qt5_DIR` to the corresponding CMake package directory.

## Build

From the project root, use the following example build commands for your platform:

### macOS

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/macos"
cmake --build build
```

### Linux

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/gcc_64"
cmake --build build
```

### Windows (PowerShell)

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64"
cmake --build build --config Release
```

> Replace `/path/to/Qt/6.x.x/...` or `C:/Qt/6.x.x/...` with the actual path to your Qt installation for your OS and compiler.

## Run

After building:

```bash
./build/minibi
```


## Project Layout

```text
.
├── CMakeLists.txt
└── src
    ├── MainWindow.h
    ├── MainWindow.cpp
    └── main.cpp
```

