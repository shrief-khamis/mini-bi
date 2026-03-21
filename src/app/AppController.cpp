#include "app/AppController.h"

#include "app/MainWindow.h"
#include "logging/Log.h"
#include "ui/widgets/Canvas.h"
#include "ui/widgets/ControlPanel.h"

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
    QMessageBox::information(m_mainWindow, "Load Data", "Load Data clicked.");
}

void AppController::onPlotRequested() {
    Log::info(LogCategory::UI, QStringLiteral("Plot action requested"));
    m_mainWindow->canvasPanel()->renderPlaceholderPlot();
}
