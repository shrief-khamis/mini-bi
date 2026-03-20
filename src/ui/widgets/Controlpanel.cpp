#include "ui/widgets/Controlpanel.h"

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

Controlpanel::Controlpanel(QWidget* parent)
    : QWidget(parent) {
    configureUi();
    wireSignals();
}

void Controlpanel::configureUi() {
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

void Controlpanel::wireSignals() {
    connect(m_loadDataButton, &QPushButton::clicked, this, &Controlpanel::loadDataRequested);
    connect(m_plotButton, &QPushButton::clicked, this, &Controlpanel::plotRequested);
}
