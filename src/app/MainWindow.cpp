#include "app/MainWindow.h"

#include "logging/Log.h"
#include "ui/widgets/Canvas.h"
#include "ui/widgets/ControlPanel.h"

#include <QHBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    configureUi();
}

ControlPanel* MainWindow::controlPanel() const {
    return m_controlPanel;
}

Canvas* MainWindow::canvasPanel() const {
    return m_canvasPanel;
}

void MainWindow::configureUi() {
    setWindowTitle("MiniBI Qt App");
    resize(980, 620);

    Log::info(LogCategory::App, QStringLiteral("Main window UI initializing"));

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* layout = new QHBoxLayout(central);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    m_controlPanel = new ControlPanel(central);
    m_canvasPanel = new Canvas(central);

    layout->addWidget(m_controlPanel, 1);
    layout->addWidget(m_canvasPanel, 4);

    Log::info(LogCategory::App, QStringLiteral("Main window UI ready"));
}
