#include "plot/PlaceholderPlot.h"

#include <QPainter>

#include <algorithm>

PlaceholderPlot::PlaceholderPlot(bool dataLoaded)
    : m_dataLoaded(dataLoaded) {}

void PlaceholderPlot::render(QPainter& painter, const QRect& canvasArea) {
    drawStatusText(painter, canvasArea, m_dataLoaded);
}

void PlaceholderPlot::drawStatusText(QPainter& painter, const QRect& canvasArea, bool dataLoaded) const {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(90, 90, 100));
    QFont font = painter.font();
    font.setPointSizeF(std::max(12.0, font.pointSizeF() + 2));
    painter.setFont(font);
    const QString text =
        dataLoaded ? QStringLiteral("Data Loaded") : QStringLiteral("No data loaded");
    painter.drawText(canvasArea, static_cast<int>(Qt::AlignCenter), text);
}
