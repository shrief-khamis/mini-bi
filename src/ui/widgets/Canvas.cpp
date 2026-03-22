#include "ui/widgets/Canvas.h"

#include <QPaintEvent>
#include <QPainter>

Canvas::Canvas(QWidget* parent)
    : QWidget(parent) {
    configureUi();
}

void Canvas::configureUi() {
    setAutoFillBackground(false);
    setStyleSheet(
        "Canvas {"
        "  border: 1px solid #bcbcbc;"
        "  border-radius: 10px;"
        "  background-color: #ffffff;"
        "}"
    );
}

void Canvas::renderPlot(std::unique_ptr<PlotRenderer> renderer) {
    m_renderer = std::move(renderer);
    update();
}

void Canvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    const QRect drawableArea = rect().adjusted(16, 16, -16, -16);

    if (m_renderer) {
        m_renderer->render(painter, drawableArea);
    }
}
