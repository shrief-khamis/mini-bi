#include "ui/dialogs/ImportPreviewDialog.h"

#include "ui/models/TableViewModel.h"

#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

ImportPreviewDialog::ImportPreviewDialog(QWidget* parent)
    : QDialog(parent) {
    configureUi();
    wireSignals();
}

void ImportPreviewDialog::setPreviewData(const QString& fileName, const DataTable& dataTable) {
    m_previewTable = dataTable;
    m_tableModel->setDataTable(m_previewTable);
    m_summaryLabel->setText(
        QStringLiteral("File: %1 | Columns: %2 | Rows: %3")
            .arg(fileName)
            .arg(m_previewTable.columnCount())
            .arg(m_previewTable.rowCount())
    );
}

const DataTable& ImportPreviewDialog::previewTable() const {
    return m_previewTable;
}

void ImportPreviewDialog::configureUi() {
    setWindowTitle("Import Preview");
    resize(900, 560);

    auto* layout = new QVBoxLayout(this);

    m_summaryLabel = new QLabel(this);
    layout->addWidget(m_summaryLabel);

    m_tableView = new QTableView(this);
    m_tableModel = new TableViewModel(this);
    m_tableView->setModel(m_tableModel);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    layout->addWidget(m_tableView);

    auto* buttonBox = new QDialogButtonBox(this);
    auto* confirmButton = buttonBox->addButton("Confirm", QDialogButtonBox::AcceptRole);
    auto* cancelButton = buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    connect(confirmButton, &QPushButton::clicked, this, &ImportPreviewDialog::onConfirmClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ImportPreviewDialog::onCancelClicked);
    layout->addWidget(buttonBox);
}

void ImportPreviewDialog::wireSignals() {}

void ImportPreviewDialog::onConfirmClicked() {
    emit confirmRequested();
    accept();
}

void ImportPreviewDialog::onCancelClicked() {
    emit cancelRequested();
    reject();
}
