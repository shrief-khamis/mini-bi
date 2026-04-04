#include "plot/core/PlotData.h"

#include <variant>

std::optional<double> PlotData::cellToDouble(const CellValue& cell) {
    if (std::holds_alternative<std::monostate>(cell))
        return std::nullopt;
    if (std::holds_alternative<double>(cell))
        return std::get<double>(cell);
    if (std::holds_alternative<int>(cell))
        return static_cast<double>(std::get<int>(cell));
    if (std::holds_alternative<bool>(cell))
        return std::get<bool>(cell) ? 1.0 : 0.0;
    if (std::holds_alternative<QString>(cell)) {
        bool ok = false;
        const double v = std::get<QString>(cell).toDouble(&ok);
        if (ok)
            return v;
    }
    return std::nullopt;
}

std::optional<QString> PlotData::cellToCategory(const CellValue& cell) {
    if (std::holds_alternative<std::monostate>(cell))
        return std::nullopt;
    if (std::holds_alternative<QString>(cell)) {
        const QString& s = std::get<QString>(cell);
        if (s.trimmed().isEmpty())
            return std::nullopt;
        return s.trimmed();
    }
    if (std::holds_alternative<int>(cell))
        return QString::number(std::get<int>(cell));
    if (std::holds_alternative<double>(cell))
        return QString::number(std::get<double>(cell), 'g', 6);
    if (std::holds_alternative<bool>(cell))
        return std::get<bool>(cell) ? QStringLiteral("true") : QStringLiteral("false");
    return std::nullopt;
}
