#pragma once

#include "data/table/DataTable.h"

#include <QAbstractTableModel>

class TableViewModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TableViewModel(QObject* parent = nullptr);
    ~TableViewModel() override = default;

    void setDataTable(const DataTable& dataTable);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    static QString cellValueToString(const CellValue& value);

    DataTable m_dataTable;
};
