#pragma once

#include "data/table/DataTable.h"

#include <QString>
#include <optional>

/**
 * Plot-facing helpers for interpreting table cells (parsing, labels).
 */
class PlotData {
public:
    static std::optional<double> cellToDouble(const CellValue& cell);
    static std::optional<QString> cellToCategory(const CellValue& cell);
};
