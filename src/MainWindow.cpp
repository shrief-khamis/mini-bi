#include "MainWindow.h"

#include "Canvas.h"
#include "Controlpanel.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    configureUi();
}

void MainWindow::configureUi() {
    setWindowTitle("MiniBI Qt App");
    resize(980, 620);

    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* layout = new QHBoxLayout(central);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(12);

    m_controlPanel = new Controlpanel(central);
    m_canvasPanel = new Canvas(central);

    layout->addWidget(m_controlPanel, 1);
    layout->addWidget(m_canvasPanel, 4);

    connect(m_controlPanel, &Controlpanel::loadDataRequested, this, [this]() {
        QMessageBox::information(this, "Load Data", "Load Data clicked.");
    });

    connect(m_controlPanel, &Controlpanel::plotRequested, this, [this]() {
        QMessageBox::information(this, "Plot", "Plot clicked.");
    });
}
