#include "ui/widgets/Canvas.h"

#include <QFrame>
#include <QVBoxLayout>

Canvas::Canvas(QWidget* parent)
    : QWidget(parent) {
    configureUi();
}

void Canvas::configureUi() {
    auto* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    auto* frame = new QFrame(this);
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setStyleSheet(
        "QFrame {"
        "  border: 1px solid #bcbcbc;"
        "  border-radius: 10px;"
        "  background-color: #ffffff;"
        "}"
    );

    auto* frameLayout = new QVBoxLayout(frame);
    frameLayout->setContentsMargins(12, 12, 12, 12);

    rootLayout->addWidget(frame);
}
