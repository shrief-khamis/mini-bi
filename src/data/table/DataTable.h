#pragma once

#include <QString>

#include <variant>
#include <vector>

using CellValue = std::variant<std::monostate, int, double, QString, bool>;

enum class DataType {
    Empty,
    Integer,
    Double,
    String,
    Bool,
};

struct Column {
    QString name;
    DataType declaredType = DataType::Empty;
    std::vector<CellValue> values;
};

class DataTable {
public:
    DataTable() = default;
    ~DataTable() = default;

    void addColumn(const Column& column);
    void addColumn(Column&& column);

    int rowCount() const;
    int columnCount() const;

    const std::vector<Column>& columns() const;

private:
    std::vector<Column> m_columns;
};
