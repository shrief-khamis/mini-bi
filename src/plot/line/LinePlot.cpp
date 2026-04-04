#include "plot/line/LinePlot.h"

#include "plot/core/PlotData.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPen>
#include <QPolygonF>
#include <QRectF>
#include <QString>

#include <algorithm>
#include <cmath>

namespace {

constexpr int kMarginLeft = 56;
constexpr int kMarginRight = 16;
constexpr int kMarginTop = 16;
constexpr int kMarginBottom = 52;

double expandIfEqual(double minV, double maxV) {
    if (minV == maxV) {
        const double pad = std::abs(minV) > 1e-9 ? std::abs(minV) * 0.05 : 0.5;
        return pad;
    }
    return (maxV - minV) * 0.05;
}

} // namespace

LinePlot::LinePlot(const DataTable& table) {
    setData(table);
}

void LinePlot::setData(const DataTable& table) {
    m_x.clear();
    m_y.clear();
    m_xLabel.clear();
    m_yLabel.clear();

    if (table.columnCount() < 2)
        return;

    const std::vector<Column>& cols = table.columns();
    const Column& xCol = cols[0];
    const Column& yCol = cols[1];
    m_xLabel = xCol.name.isEmpty() ? QStringLiteral("X") : xCol.name;
    m_yLabel = yCol.name.isEmpty() ? QStringLiteral("Y") : yCol.name;

    const int n = std::min(static_cast<int>(xCol.values.size()), static_cast<int>(yCol.values.size()));
    const int rows = std::min(n, table.rowCount());
    m_x.reserve(static_cast<std::size_t>(rows));
    m_y.reserve(static_cast<std::size_t>(rows));

    for (int i = 0; i < rows; ++i) {
        const auto xOpt = PlotData::cellToDouble(xCol.values[static_cast<std::size_t>(i)]);
        const auto yOpt = PlotData::cellToDouble(yCol.values[static_cast<std::size_t>(i)]);
        if (!xOpt.has_value() || !yOpt.has_value())
            continue;
        if (!std::isfinite(*xOpt) || !std::isfinite(*yOpt))
            continue;
        m_x.push_back(*xOpt);
        m_y.push_back(*yOpt);
    }
}

LinePlot::Bounds LinePlot::computeDataBounds() const {
    Bounds b;
    if (m_x.empty()) {
        b.isEmpty = true;
        return b;
    }
    b.isEmpty = false;
    b.minX = b.maxX = m_x[0];
    b.minY = b.maxY = m_y[0];
    for (std::size_t i = 0; i < m_x.size(); ++i) {
        b.minX = std::min(b.minX, m_x[i]);
        b.maxX = std::max(b.maxX, m_x[i]);
        b.minY = std::min(b.minY, m_y[i]);
        b.maxY = std::max(b.maxY, m_y[i]);
    }
    return b;
}

LinePlot::Bounds LinePlot::paddedBounds(const Bounds& b) {
    if (b.isEmpty)
        return b;
    Bounds out = b;
    const double px = expandIfEqual(b.minX, b.maxX);
    const double py = expandIfEqual(b.minY, b.maxY);
    out.minX -= px;
    out.maxX += px;
    out.minY -= py;
    out.maxY += py;
    return out;
}

QRectF LinePlot::plotRectForCanvas(const QRect& canvasArea) const {
    return QRectF(
        static_cast<double>(canvasArea.left() + kMarginLeft),
        static_cast<double>(canvasArea.top() + kMarginTop),
        static_cast<double>(canvasArea.width() - kMarginLeft - kMarginRight),
        static_cast<double>(canvasArea.height() - kMarginTop - kMarginBottom));
}

void LinePlot::drawCanvasBackground(QPainter& painter, const QRect& canvasArea) const {
    painter.fillRect(canvasArea, QColor(252, 252, 255));
}

void LinePlot::drawEmptyState(QPainter& painter, const QRect& canvasArea) const {
    painter.setPen(QColor(100, 100, 110));
    painter.drawText(canvasArea, static_cast<int>(Qt::AlignCenter),
                       QStringLiteral("No numeric points in the first two columns."));
}

void LinePlot::drawPlotRegionBackground(QPainter& painter, const QRectF& plotRect) const {
    painter.fillRect(plotRect, Qt::white);
    painter.setPen(QPen(QColor(210, 210, 220), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(plotRect);
}

void LinePlot::drawAxes(QPainter& painter, const QRectF& plotRect) const {
    painter.setPen(QPen(QColor(60, 60, 70), 1.5));
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    painter.drawLine(plotRect.bottomLeft(), plotRect.topLeft());
}

void LinePlot::drawGridLines(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                             const std::vector<double>& xTicks, const std::vector<double>& yTicks) const {
    painter.setPen(QPen(QColor(230, 230, 235), 1, Qt::DotLine));
    for (double xv : xTicks) {
        if (xv < dataBounds.minX || xv > dataBounds.maxX)
            continue;
        const QPointF p1 = dataToPixel(xv, dataBounds.minY, plotRect, dataBounds);
        const QPointF p2 = dataToPixel(xv, dataBounds.maxY, plotRect, dataBounds);
        painter.drawLine(p1, p2);
    }
    for (double yv : yTicks) {
        if (yv < dataBounds.minY || yv > dataBounds.maxY)
            continue;
        const QPointF p1 = dataToPixel(dataBounds.minX, yv, plotRect, dataBounds);
        const QPointF p2 = dataToPixel(dataBounds.maxX, yv, plotRect, dataBounds);
        painter.drawLine(p1, p2);
    }
}

void LinePlot::drawTickMarksAndLabels(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                                      const std::vector<double>& xTicks,
                                      const std::vector<double>& yTicks) const {
    painter.setPen(QColor(50, 50, 60));
    QFont font = painter.font();
    font.setPointSizeF(std::max(7.0, font.pointSizeF() - 1));
    painter.setFont(font);
    const QFontMetricsF fm(font);

    for (double xv : xTicks) {
        if (xv < dataBounds.minX || xv > dataBounds.maxX)
            continue;
        const QPointF bottom = dataToPixel(xv, dataBounds.minY, plotRect, dataBounds);
        painter.drawLine(bottom, bottom + QPointF(0, 5));
        const QString text = QString::number(xv, 'g', 4);
        const double tw = fm.horizontalAdvance(text);
        painter.drawText(QPointF(bottom.x() - tw / 2.0, plotRect.bottom() + 4 + fm.ascent()), text);
    }

    for (double yv : yTicks) {
        if (yv < dataBounds.minY || yv > dataBounds.maxY)
            continue;
        const QPointF left = dataToPixel(dataBounds.minX, yv, plotRect, dataBounds);
        painter.drawLine(left, left - QPointF(5, 0));
        const QString text = QString::number(yv, 'g', 4);
        const double tw = fm.horizontalAdvance(text);
        painter.drawText(QPointF(plotRect.left() - tw - 8, left.y() + fm.ascent() / 2.0 - 2), text);
    }
}

void LinePlot::drawAxisTitles(QPainter& painter, const QRectF& plotRect, const QRect& canvasArea) const {
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(QColor(40, 40, 50));
    const QFontMetricsF fm(font);

    painter.drawText(
        QRectF(plotRect.left(), static_cast<double>(canvasArea.bottom() - kMarginBottom + 4),
               plotRect.width(), static_cast<double>(kMarginBottom - 4)),
        static_cast<int>(Qt::AlignHCenter | Qt::AlignTop),
        m_xLabel);

    painter.save();
    painter.translate(static_cast<double>(canvasArea.left() + 12),
                      plotRect.center().y());
    painter.rotate(-90);
    painter.drawText(QRectF(-200, -fm.height(), 400, fm.height()), static_cast<int>(Qt::AlignCenter), m_yLabel);
    painter.restore();
}

void LinePlot::drawLineSeries(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds) const {
    constexpr double r = 3.5;
    painter.setPen(QPen(QColor(52, 88, 158), 2));
    if (m_x.size() == 1) {
        painter.setBrush(QColor(80, 120, 200, 200));
        const QPointF p = dataToPixel(m_x[0], m_y[0], plotRect, dataBounds);
        painter.drawEllipse(p, r, r);
        return;
    }

    QPolygonF poly;
    poly.reserve(static_cast<int>(m_x.size()));
    for (std::size_t i = 0; i < m_x.size(); ++i)
        poly.append(dataToPixel(m_x[i], m_y[i], plotRect, dataBounds));
    painter.setBrush(Qt::NoBrush);
    painter.drawPolyline(poly);
}

QPointF LinePlot::dataToPixel(double x, double y, const QRectF& plotRect, const Bounds& b) const {
    const double dx = b.maxX - b.minX;
    const double dy = b.maxY - b.minY;
    const double nx = dx > 0 ? (x - b.minX) / dx : 0.5;
    const double ny = dy > 0 ? (y - b.minY) / dy : 0.5;
    const double px = plotRect.left() + nx * plotRect.width();
    const double py = plotRect.bottom() - ny * plotRect.height();
    return QPointF(px, py);
}

std::vector<double> LinePlot::computeTickValues(double minV, double maxV, int maxTicks) {
    std::vector<double> ticks;
    if (maxTicks < 2 || minV > maxV)
        return ticks;
    if (minV == maxV) {
        ticks.push_back(minV);
        return ticks;
    }
    const int n = std::max(2, maxTicks);
    for (int i = 0; i < n; ++i) {
        const double t = static_cast<double>(i) / static_cast<double>(n - 1);
        ticks.push_back(minV + t * (maxV - minV));
    }
    return ticks;
}

void LinePlot::render(QPainter& painter, const QRect& canvasArea) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawCanvasBackground(painter, canvasArea);

    if (m_x.empty()) {
        drawEmptyState(painter, canvasArea);
        return;
    }

    Bounds b = computeDataBounds();
    b = paddedBounds(b);
    const QRectF plotRect = plotRectForCanvas(canvasArea);

    drawPlotRegionBackground(painter, plotRect);
    drawAxes(painter, plotRect);

    const std::vector<double> xTicks = computeTickValues(b.minX, b.maxX, 6);
    const std::vector<double> yTicks = computeTickValues(b.minY, b.maxY, 6);
    drawGridLines(painter, plotRect, b, xTicks, yTicks);
    drawTickMarksAndLabels(painter, plotRect, b, xTicks, yTicks);
    drawLineSeries(painter, plotRect, b);
    drawAxisTitles(painter, plotRect, canvasArea);
}
