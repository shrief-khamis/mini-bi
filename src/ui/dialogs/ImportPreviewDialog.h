#pragma once

#include "data/table/DataTable.h"

#include <QDialog>

class QLabel;
class QTableView;
class TableViewModel;

class ImportPreviewDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImportPreviewDialog(QWidget* parent = nullptr);
    ~ImportPreviewDialog() override = default;

    void setPreviewData(const QString& fileName, const DataTable& dataTable);

signals:
    void confirmRequested();
    void cancelRequested();

private:
    void configureUi();
    void wireSignals();
    void onConfirmClicked();
    void onCancelClicked();

    QLabel* m_summaryLabel = nullptr;
    QTableView* m_tableView = nullptr;
    TableViewModel* m_tableModel = nullptr;
};
