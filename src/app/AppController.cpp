#include "app/AppController.h"

#include "app/MainWindow.h"
#include "data/ingestion/CSVReader.h"
#include "logging/Log.h"
#include "plot/PlaceholderPlot.h"
#include "plot/scatter/ScatterPlot.h"
#include "ui/dialogs/ImportPreviewDialog.h"
#include "ui/widgets/Canvas.h"
#include "ui/widgets/ControlPanel.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include <memory>

AppController::AppController(MainWindow* mainWindow, QObject* parent)
    : QObject(parent), m_mainWindow(mainWindow) {
    wireSignals();
    m_mainWindow->canvasPanel()->renderPlot(std::make_unique<PlaceholderPlot>(false));
}

void AppController::wireSignals() {
    connect(m_mainWindow->controlPanel(), &ControlPanel::loadDataRequested,
            this, &AppController::onLoadDataRequested);
    connect(m_mainWindow->controlPanel(), &ControlPanel::plotRequested,
            this, &AppController::onPlotRequested);
    connect(m_mainWindow->controlPanel(), &ControlPanel::wipeRequested,
            this, &AppController::onWipeRequested);
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
    Log::info(
        LogCategory::App,
        QStringLiteral("Loaded data file '%1' (%2 columns, %3 rows)")
            .arg(fileName)
            .arg(table->columnCount())
            .arg(table->rowCount())
    );

    auto* dialog = new ImportPreviewDialog(m_mainWindow);
    dialog->setPreviewData(fileName, table.value());
    connect(dialog, &ImportPreviewDialog::confirmRequested, this, [this, dialog]() {
        applyConfirmedImport(dialog->previewTable());
    });
    connect(dialog, &ImportPreviewDialog::cancelRequested, this, &AppController::onImportPreviewCanceled);
    dialog->exec();
    dialog->deleteLater();
}

void AppController::applyConfirmedImport(const DataTable& table) {
    m_loadedData = table;
    Log::info(
        LogCategory::App,
        QStringLiteral("Import confirmed; data kept in memory (%1 columns, %2 rows)")
            .arg(table.columnCount())
            .arg(table.rowCount())
    );
    m_mainWindow->canvasPanel()->renderPlot(std::make_unique<PlaceholderPlot>(true));
}

void AppController::onPlotRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Plot action requested"));

    if (!m_loadedData.has_value()) {
        Log::warning(LogCategory::App, QStringLiteral("Plot requested but no data is loaded"));
        QMessageBox::information(m_mainWindow, "Plot", "Load and confirm data first.");
        return;
    }

    m_mainWindow->canvasPanel()->renderPlot(std::make_unique<ScatterPlot>(*m_loadedData));
}

void AppController::onWipeRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Wipe requested"));
    m_loadedData.reset();
    m_mainWindow->canvasPanel()->renderPlot(std::make_unique<PlaceholderPlot>(false));
}

void AppController::onImportPreviewCanceled() {
    Log::info(LogCategory::UI, QStringLiteral("Import preview canceled"));
}
