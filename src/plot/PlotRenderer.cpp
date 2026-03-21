#include "plot/PlotRenderer.h"

#include <QPainter>
#include <QRect>

#include <algorithm>

void PlotRenderer::drawCenteredCircle(QPainter& painter, const QRect& canvasArea) const {
    const int radius = std::max(24, std::min(canvasArea.width(), canvasArea.height()) / 6);
    const QPoint center = canvasArea.center();

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(QColor(255, 0, 0), 2));
    painter.setBrush(QColor(0, 0, 255, 255));
    painter.drawEllipse(center, radius, radius);
}
