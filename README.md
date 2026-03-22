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

Source is grouped by concern. **Include paths are rooted at `src/`** (e.g. `#include "app/MainWindow.h"`, `#include "plot/line/LinePlot.h"`, `#include "data/table/DataTable.h"`).

```text
.
├── CMakeLists.txt              # Top-level executable + subdirectory order
├── README.md
├── input-data/                 # Sample CSVs for manual testing (not part of the build)
│   ├── ascending_series_100.csv
│   ├── correlated_scatter_1000.csv
│   ├── sample_mixed_types.csv
│   └── sample_xy_100.csv
└── src
    ├── main.cpp                # Application entry point
    ├── app/                    # Main window + cross-cutting actions (minibi_app)
    │   ├── CMakeLists.txt
    │   ├── AppController.h
    │   ├── AppController.cpp
    │   ├── MainWindow.h
    │   └── MainWindow.cpp
    ├── data/                   # Tabular model + CSV ingestion (minibi_data)
    │   ├── CMakeLists.txt
    │   ├── ingestion/
    │   │   ├── CSVReader.h
    │   │   └── CSVReader.cpp
    │   └── table/
    │       ├── DataTable.h
    │       └── DataTable.cpp
    ├── logging/                # Logging helpers (minibi_logging)
    │   ├── CMakeLists.txt
    │   ├── Log.h
    │   └── Log.cpp
    ├── plot/                   # Plot renderers used by the canvas (minibi_plot)
    │   ├── CMakeLists.txt
    │   ├── PlaceholderPlot.h
    │   ├── PlaceholderPlot.cpp
    │   ├── core/
    │   │   ├── PlotData.h
    │   │   └── PlotRenderer.h
    │   ├── line/
    │   │   ├── LinePlot.h
    │   │   └── LinePlot.cpp
    │   └── scatter/
    │       ├── ScatterPlot.h
    │       └── ScatterPlot.cpp
    └── ui/                     # Widgets, dialogs, Qt view models (minibi_ui)
        ├── CMakeLists.txt
        ├── dialogs/
        │   ├── ImportPreviewDialog.h
        │   └── ImportPreviewDialog.cpp
        ├── models/
        │   ├── TableViewModel.h
        │   └── TableViewModel.cpp
        └── widgets/
            ├── Canvas.h
            ├── Canvas.cpp
            ├── ControlPanel.h
            └── ControlPanel.cpp
```

## CMake targets

| Target           | Role |
|------------------|------|
| `minibi`         | Executable; links `minibi_app` |
| `minibi_app`     | Static library: `AppController`, `MainWindow` |
| `minibi_ui`      | Static library: `Canvas`, `ControlPanel`, `ImportPreviewDialog`, `TableViewModel`, … |
| `minibi_plot`    | Static library: `PlotRenderer` implementations (`PlaceholderPlot`, `LinePlot`, `ScatterPlot`, …) |
| `minibi_data`    | Static library: `DataTable`, `CSVReader` |
| `minibi_logging` | Static library: `Log` |

The top-level `CMakeLists.txt` adds subdirectories in dependency order: **`logging` → `data` → `plot` → `ui` → `app`**. `minibi_plot` links `minibi_data`; `minibi_ui` links `minibi_plot` and `minibi_logging`; `minibi_app` links `minibi_ui` and `minibi_data`.

## Notes

This project is still under development and in an early foundation phase.
