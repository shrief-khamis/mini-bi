#include "data/table/DataTable.h"

#include <algorithm>

void DataTable::addColumn(const Column& column) {
    m_columns.push_back(column);
}

void DataTable::addColumn(Column&& column) {
    m_columns.push_back(std::move(column));
}

int DataTable::rowCount() const {
    std::size_t maxRows = 0;
    for (const Column& column : m_columns) {
        maxRows = std::max(maxRows, column.values.size());
    }
    return static_cast<int>(maxRows);
}

int DataTable::columnCount() const {
    return static_cast<int>(m_columns.size());
}

const std::vector<Column>& DataTable::columns() const {
    return m_columns;
}
