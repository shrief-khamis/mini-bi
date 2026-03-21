#include "ui/widgets/ControlPanel.h"

#include "logging/Log.h"

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent) {
    configureUi();
    wireSignals();
}

void ControlPanel::configureUi() {
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet(
        "QFrame {"
        "  border: 1px solid #3f4349;"
        "  border-radius: 10px;"
        "  background-color: #1f2329;"
        "}"
        "QPushButton {"
        "  background-color: #2d333b;"
        "  color: #f2f2f2;"
        "  border: 1px solid #4c525a;"
        "  border-radius: 8px;"
        "  padding: 8px 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #3a414a;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #232a33;"
        "}"
    );

    auto* frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(12, 12, 12, 12);
    frameLayout->setSpacing(10);

    m_loadDataButton = new QPushButton("Load Data", frame);
    m_plotButton = new QPushButton("Plot", frame);

    frameLayout->addWidget(m_loadDataButton);
    frameLayout->addWidget(m_plotButton);
    frameLayout->addStretch();

    rootLayout->addWidget(frame);
}

void ControlPanel::wireSignals() {
    connect(m_loadDataButton, &QPushButton::clicked, this, &ControlPanel::onLoadDataButtonClicked);
    connect(m_plotButton, &QPushButton::clicked, this, &ControlPanel::onPlotButtonClicked);
}

void ControlPanel::onLoadDataButtonClicked() {
    Log::debug(LogCategory::UI, QStringLiteral("Load Data button clicked"));
    // Placeholder for local UI-side action before forwarding to app flow.
    emit loadDataRequested();
}

void ControlPanel::onPlotButtonClicked() {
    Log::debug(LogCategory::UI, QStringLiteral("Plot button clicked"));
    // Placeholder for local UI-side action before forwarding to app flow.
    emit plotRequested();
}
