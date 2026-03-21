#include "app/AppController.h"

#include "app/MainWindow.h"
#include "data/ingestion/CSVReader.h"
#include "logging/Log.h"
#include "ui/widgets/Canvas.h"
#include "ui/widgets/ControlPanel.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

AppController::AppController(MainWindow* mainWindow, QObject* parent)
    : QObject(parent), m_mainWindow(mainWindow) {
    wireSignals();
}

void AppController::wireSignals() {
    connect(m_mainWindow->controlPanel(), &ControlPanel::loadDataRequested,
            this, &AppController::onLoadDataRequested);
    connect(m_mainWindow->controlPanel(), &ControlPanel::plotRequested,
            this, &AppController::onPlotRequested);
}

void AppController::onLoadDataRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Load Data action requested"));

    const QString selectedFilePath = QFileDialog::getOpenFileName(
        m_mainWindow,
        "Select Data File",
        QString(),
        "CSV files (*.csv);;All files (*.*)"
    );

    if (selectedFilePath.isEmpty()) {
        Log::info(LogCategory::UI, QStringLiteral("Load Data canceled"));
        return;
    }

    CSVReader reader;
    QString errorMessage;
    const auto table = reader.readFile(selectedFilePath, &errorMessage);
    if (!table.has_value()) {
        Log::warning(
            LogCategory::App,
            QStringLiteral("Data ingestion failed for '%1': %2").arg(selectedFilePath, errorMessage)
        );
        QMessageBox::warning(m_mainWindow, "Data Load Failed", errorMessage);
        return;
    }

    const QString fileName = QFileInfo(selectedFilePath).fileName();
    const QString details = QStringLiteral("File: %1\nColumns: %2\nRows: %3")
                                .arg(fileName)
                                .arg(table->columnCount())
                                .arg(table->rowCount());

    Log::info(
        LogCategory::App,
        QStringLiteral("Loaded data file '%1' (%2 columns, %3 rows)")
            .arg(fileName)
            .arg(table->columnCount())
            .arg(table->rowCount())
    );

    auto* dialog = new QMessageBox(m_mainWindow);
    dialog->setWindowTitle("Data Loaded");
    dialog->setText(details);
    dialog->setIcon(QMessageBox::Information);
    dialog->addButton("Confirm", QMessageBox::AcceptRole);
    dialog->exec();
}

void AppController::onPlotRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Plot action requested"));
    m_mainWindow->canvasPanel()->renderPlaceholderPlot();
}
