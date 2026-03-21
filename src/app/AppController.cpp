#include "app/AppController.h"

#include "app/MainWindow.h"
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
    const QString selectedFilePath = QFileDialog::getOpenFileName(
        m_mainWindow,
        "Select Data File"
    );

    if (selectedFilePath.isEmpty()) {
        Log::info(LogCategory::UI, QStringLiteral("Load Data canceled"));
        return;
    }

    const QString fileName = QFileInfo(selectedFilePath).fileName();
    auto* dialog = new QMessageBox(m_mainWindow);
    dialog->setWindowTitle("File Selected");
    dialog->setText(fileName);
    dialog->setIcon(QMessageBox::Information);
    dialog->addButton("Confirm", QMessageBox::AcceptRole);
    dialog->exec();
}

void AppController::onPlotRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Plot action requested"));
    m_mainWindow->canvasPanel()->renderPlaceholderPlot();
}
