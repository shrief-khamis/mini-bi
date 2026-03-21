#include "data/ingestion/CSVReader.h"

#include "data/table/DataTable.h"
#include "logging/Log.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <utility>
#include <vector>

namespace {

std::vector<QString> splitSimpleCsvLine(const QString& line) {
    const QStringList parts = line.split(',', Qt::KeepEmptyParts);
    std::vector<QString> values;
    values.reserve(parts.size());
    for (const QString& part : parts) {
        values.push_back(part.trimmed());
    }
    return values;
}

bool parseBool(const QString& value, bool& outValue) {
    const QString normalized = value.trimmed().toLower();
    if (normalized == "true") {
        outValue = true;
        return true;
    }
    if (normalized == "false") {
        outValue = false;
        return true;
    }
    return false;
}

CellValue parseCellValue(const QString& rawValue, DataType& outType) {
    const QString value = rawValue.trimmed();
    if (value.isEmpty()) {
        outType = DataType::Empty;
        return std::monostate{};
    }

    bool boolValue = false;
    if (parseBool(value, boolValue)) {
        outType = DataType::Bool;
        return boolValue;
    }

    bool intOk = false;
    const int intValue = value.toInt(&intOk);
    if (intOk) {
        outType = DataType::Integer;
        return intValue;
    }

    bool doubleOk = false;
    const double doubleValue = value.toDouble(&doubleOk);
    if (doubleOk) {
        outType = DataType::Double;
        return doubleValue;
    }

    outType = DataType::String;
    return value;
}

int dataTypeRank(const DataType type) {
    switch (type) {
    case DataType::Empty:
        return 0;
    case DataType::Bool:
        return 1;
    case DataType::Integer:
        return 2;
    case DataType::Double:
        return 3;
    case DataType::String:
        return 4;
    }
    return 4;
}

bool openCsvFile(const QString& filePath, QFile& file, QString* errorMessage) {
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const QString error = QStringLiteral("Failed to open CSV file: %1").arg(file.errorString());
        Log::warning(LogCategory::App, error);
        if (errorMessage != nullptr) {
            *errorMessage = error;
        }
        return false;
    }
    return true;
}

std::vector<QString> readHeaders(QTextStream& stream) {
    const QString headerLine = stream.readLine();
    std::vector<QString> headers = splitSimpleCsvLine(headerLine);
    if (headers.empty()) {
        headers.push_back(QStringLiteral("Column 1"));
    }
    return headers;
}

std::vector<Column> initializeColumns(const std::vector<QString>& headers) {
    std::vector<Column> columns;
    columns.reserve(headers.size());
    for (const QString& header : headers) {
        Column column;
        column.name = header.isEmpty() ? QStringLiteral("Unnamed") : header;
        column.declaredType = DataType::Empty;
        columns.push_back(std::move(column));
    }
    return columns;
}

void appendParsedRowToColumns(const std::vector<QString>& rowValues, std::vector<Column>& columns) {
    for (std::size_t col = 0; col < columns.size(); ++col) {
        const QString rawValue = col < rowValues.size() ? rowValues[col] : QString();
        DataType parsedType = DataType::Empty;
        CellValue value = parseCellValue(rawValue, parsedType);
        columns[col].values.push_back(std::move(value));
        if (dataTypeRank(parsedType) > dataTypeRank(columns[col].declaredType)) {
            columns[col].declaredType = parsedType;
        }
    }
}

void ingestRows(QTextStream& stream, std::vector<Column>& columns) {
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        if (line.trimmed().isEmpty()) {
            continue;
        }
        const std::vector<QString> rowValues = splitSimpleCsvLine(line);
        appendParsedRowToColumns(rowValues, columns);
    }
}

DataTable buildDataTable(std::vector<Column>&& columns) {
    DataTable table;
    for (Column& column : columns) {
        table.addColumn(std::move(column));
    }
    return table;
}

}  // anonymous namespace of helper functions to keep the class interface clean

std::optional<DataTable> CSVReader::readFile(const QString& filePath, QString* errorMessage) const {
    Log::info(LogCategory::App, QStringLiteral("CSVReader: opening file %1").arg(filePath));

    QFile file;
    if (!openCsvFile(filePath, file, errorMessage)) {
        return std::nullopt;
    }

    QTextStream stream(&file);
    if (stream.atEnd()) {
        const QString error = QStringLiteral("CSV file is empty.");
        Log::warning(LogCategory::App, error);
        if (errorMessage != nullptr) {
            *errorMessage = error;
        }
        return std::nullopt;
    }

    const std::vector<QString> headers = readHeaders(stream);
    std::vector<Column> columns = initializeColumns(headers);
    ingestRows(stream, columns);
    DataTable table = buildDataTable(std::move(columns));

    Log::info(
        LogCategory::App,
        QStringLiteral("CSVReader: loaded %1 (%2 columns, %3 rows)")
            .arg(QFileInfo(filePath).fileName())
            .arg(table.columnCount())
            .arg(table.rowCount())
    );

    return table;
}
