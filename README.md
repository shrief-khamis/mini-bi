# miniBI

`miniBI` is a lightweight desktop business intelligence (BI) application built with Qt.

The goal is to provide a fast, approachable way for users to create simple plots and explore data without relying on heavy, multi GBs analytics tools. The project is intentionally evolving, so architecture and features will be refined iteratively.


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

Replace `/path/to/Qt/6.x.x/...` or `C:/Qt/6.x.x/...` with the actual path to your Qt installation for your OS and compiler.

## Run

After building:

```bash
./build/minibi
```


## Project layout

Source is grouped by concern. **Include paths are rooted at `src/`** (e.g. `#include "app/MainWindow.h"`, `#include "logging/Log.h"`).

```text
.
├── CMakeLists.txt                 # Top-level: executable + subdirectories
└── src
    ├── main.cpp                   # Application entry point
    ├── app/                       # Application shell (main window)
    │   ├── CMakeLists.txt         # minibi_app library
    │   ├── MainWindow.h
    │   └── MainWindow.cpp
    ├── ui/                        # UI (widgets, dialogue boxes, tables...etc)
    │   ├── CMakeLists.txt         # minibi_ui library
    │   └── widgets/
    │       ├── Canvas.h
    │       ├── Canvas.cpp
    │       ├── ControlPanel.h
    │       └── ControlPanel.cpp
    └── logging/                   # Logging helpers
        ├── CMakeLists.txt         # minibi_logging library
        ├── Log.h
        └── Log.cpp
```

## CMake targets

| Target           | Role |
|------------------|------|
| `minibi`         | Executable; links `minibi_app` |
| `minibi_app`     | Static library: `MainWindow` |
| `minibi_ui`      | Static library: `Canvas`, `ControlPanel`, … |
| `minibi_logging` | Static library: `Log` (Qt **Core**) |

The UI and app libraries depend on logging where needed; the top-level `CMakeLists.txt` wires `add_subdirectory` for `logging`, then `ui`, then `app`.

## Notes

This project is still under development and in an early foundation phase.
