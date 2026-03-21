#include "ui/widgets/Canvas.h"

#include "plot/PlotRenderer.h"

#include <QPaintEvent>
#include <QPainter>
#include <QRect>

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

void Canvas::renderPlaceholderPlot() {
    m_showPlaceholderPlot = true;
    update();
}

void Canvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if (!m_showPlaceholderPlot) {
        return;
    }


    const QRect drawableArea = rect().adjusted(16, 16, -16, -16);

    PlotRenderer renderer;
    renderer.drawCenteredCircle(painter, drawableArea);
}
