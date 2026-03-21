#pragma once

#include "data/table/DataTable.h"

#include <QString>

#include <optional>

class CSVReader {
public:
    CSVReader() = default;
    ~CSVReader() = default;

    std::optional<DataTable> readFile(const QString& filePath, QString* errorMessage = nullptr) const;
};
