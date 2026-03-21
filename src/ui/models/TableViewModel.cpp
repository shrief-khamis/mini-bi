#include "ui/models/TableViewModel.h"

#include <type_traits>

TableViewModel::TableViewModel(QObject* parent)
    : QAbstractTableModel(parent) {}

void TableViewModel::setDataTable(const DataTable& dataTable) {
    beginResetModel();
    m_dataTable = dataTable;
    endResetModel();
}

int TableViewModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_dataTable.rowCount();
}

int TableViewModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return m_dataTable.columnCount();
}

QVariant TableViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return {};
    }

    const int row = index.row();
    const int col = index.column();
    const auto& columns = m_dataTable.columns();
    if (col < 0 || col >= static_cast<int>(columns.size())) {
        return {};
    }

    const Column& column = columns[static_cast<std::size_t>(col)];
    if (row < 0 || row >= static_cast<int>(column.values.size())) {
        return {};
    }

    return cellValueToString(column.values[static_cast<std::size_t>(row)]);
}

QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return {};
    }

    if (orientation == Qt::Horizontal) {
        const auto& columns = m_dataTable.columns();
        if (section < 0 || section >= static_cast<int>(columns.size())) {
            return {};
        }
        return columns[static_cast<std::size_t>(section)].name;
    }

    return section + 1;
}

QString TableViewModel::cellValueToString(const CellValue& value) {
    return std::visit(
        [](const auto& arg) -> QString {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return QString();
            } else if constexpr (std::is_same_v<T, bool>) {
                return arg ? QStringLiteral("true") : QStringLiteral("false");
            } else if constexpr (std::is_same_v<T, QString>) {
                return arg;
            } else {
                return QString::number(arg);
            }
        },
        value
    );
}
