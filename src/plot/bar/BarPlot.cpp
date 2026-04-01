#include "plot/bar/BarPlot.h"

#include <QFontMetricsF>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QString>

#include <algorithm>
#include <cmath>
#include <optional>
#include <variant>

namespace {

constexpr int kMarginLeft = 56;
constexpr int kMarginRight = 16;
constexpr int kMarginTop = 16;
constexpr int kMarginBottom = 56;

std::optional<QString> cellToCategory(const CellValue& cell) {
    if (std::holds_alternative<std::monostate>(cell))
        return std::nullopt;
    if (std::holds_alternative<QString>(cell)) {
        const QString& s = std::get<QString>(cell);
        if (s.trimmed().isEmpty())
            return std::nullopt;
        return s.trimmed();
    }
    if (std::holds_alternative<int>(cell))
        return QString::number(std::get<int>(cell));
    if (std::holds_alternative<double>(cell))
        return QString::number(std::get<double>(cell), 'g', 6);
    if (std::holds_alternative<bool>(cell))
        return std::get<bool>(cell) ? QStringLiteral("true") : QStringLiteral("false");
    return std::nullopt;
}

std::optional<double> cellToDouble(const CellValue& cell) {
    if (std::holds_alternative<double>(cell))
        return std::get<double>(cell);
    if (std::holds_alternative<int>(cell))
        return static_cast<double>(std::get<int>(cell));
    if (std::holds_alternative<bool>(cell))
        return std::get<bool>(cell) ? 1.0 : 0.0;
    if (std::holds_alternative<QString>(cell)) {
        bool ok = false;
        const double v = std::get<QString>(cell).toDouble(&ok);
        if (ok)
            return v;
    }
    return std::nullopt;
}

double expandIfEqual(double minV, double maxV) {
    if (minV == maxV) {
        const double pad = std::abs(minV) > 1e-9 ? std::abs(minV) * 0.05 : 0.5;
        return pad;
    }
    return (maxV - minV) * 0.05;
}

} // namespace

BarPlot::BarPlot(const DataTable& table) {
    setData(table);
}

void BarPlot::setData(const DataTable& table) {
    m_categories.clear();
    m_values.clear();
    m_categoryLabel.clear();
    m_valueLabel.clear();

    if (table.columnCount() < 2)
        return;

    const std::vector<Column>& cols = table.columns();
    const Column& catCol = cols[0];
    const Column& valCol = cols[1];
    m_categoryLabel = catCol.name.isEmpty() ? QStringLiteral("Category") : catCol.name;
    m_valueLabel = valCol.name.isEmpty() ? QStringLiteral("Value") : valCol.name;

    const int n = std::min(static_cast<int>(catCol.values.size()), static_cast<int>(valCol.values.size()));
    const int rows = std::min(n, table.rowCount());
    m_categories.reserve(static_cast<std::size_t>(rows));
    m_values.reserve(static_cast<std::size_t>(rows));

    for (int i = 0; i < rows; ++i) {
        const auto catOpt = cellToCategory(catCol.values[static_cast<std::size_t>(i)]);
        const auto valOpt = cellToDouble(valCol.values[static_cast<std::size_t>(i)]);
        if (!catOpt.has_value() || !valOpt.has_value())
            continue;
        if (!std::isfinite(*valOpt))
            continue;
        m_categories.push_back(*catOpt);
        m_values.push_back(*valOpt);
    }
}

BarPlot::Bounds BarPlot::computeValueBounds() const {
    Bounds b;
    if (m_values.empty()) {
        b.isEmpty = true;
        return b;
    }
    b.isEmpty = false;
    const auto [mn, mx] = std::minmax_element(m_values.begin(), m_values.end());
    b.minY = std::min(0.0, *mn);
    b.maxY = std::max(0.0, *mx);
    return b;
}

BarPlot::Bounds BarPlot::paddedYBounds(const Bounds& b) {
    if (b.isEmpty)
        return b;
    Bounds out = b;
    const double py = expandIfEqual(b.minY, b.maxY);
    // Pad upward only when the natural minimum is at or above zero so the axis does not
    // start below the bar baseline; still pad downward when there are negative values.
    if (b.minY >= 0.0)
        out.minY = 0.0;
    else
        out.minY = b.minY - py;
    out.maxY = b.maxY + py;
    return out;
}

QRectF BarPlot::plotRectForCanvas(const QRect& canvasArea) const {
    return QRectF(
        static_cast<double>(canvasArea.left() + kMarginLeft),
        static_cast<double>(canvasArea.top() + kMarginTop),
        static_cast<double>(canvasArea.width() - kMarginLeft - kMarginRight),
        static_cast<double>(canvasArea.height() - kMarginTop - kMarginBottom));
}

void BarPlot::drawCanvasBackground(QPainter& painter, const QRect& canvasArea) const {
    painter.fillRect(canvasArea, QColor(252, 252, 255));
}

void BarPlot::drawEmptyState(QPainter& painter, const QRect& canvasArea) const {
    painter.setPen(QColor(100, 100, 110));
    painter.drawText(canvasArea, static_cast<int>(Qt::AlignCenter),
                       QStringLiteral("Need a category column and a numeric column in the first two columns."));
}

void BarPlot::drawPlotRegionBackground(QPainter& painter, const QRectF& plotRect) const {
    painter.fillRect(plotRect, Qt::white);
    painter.setPen(QPen(QColor(210, 210, 220), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(plotRect);
}

void BarPlot::drawAxes(QPainter& painter, const QRectF& plotRect) const {
    painter.setPen(QPen(QColor(60, 60, 70), 1.5));
    painter.drawLine(plotRect.bottomLeft(), plotRect.bottomRight());
    painter.drawLine(plotRect.bottomLeft(), plotRect.topLeft());
}

void BarPlot::drawHorizontalGrid(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                                 const std::vector<double>& yTicks) const {
    painter.setPen(QPen(QColor(230, 230, 235), 1, Qt::DotLine));
    for (double yv : yTicks) {
        if (yv < dataBounds.minY || yv > dataBounds.maxY)
            continue;
        const double yPix = yToPixel(yv, plotRect, dataBounds);
        painter.drawLine(QPointF(plotRect.left(), yPix), QPointF(plotRect.right(), yPix));
    }
}

void BarPlot::drawYTickMarksAndLabels(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                                      const std::vector<double>& yTicks) const {
    painter.setPen(QColor(50, 50, 60));
    QFont font = painter.font();
    font.setPointSizeF(std::max(7.0, font.pointSizeF() - 1));
    painter.setFont(font);
    const QFontMetricsF fm(font);

    for (double yv : yTicks) {
        if (yv < dataBounds.minY || yv > dataBounds.maxY)
            continue;
        const double yPix = yToPixel(yv, plotRect, dataBounds);
        painter.drawLine(QPointF(plotRect.left(), yPix), QPointF(plotRect.left() - 5, yPix));
        const QString text = QString::number(yv, 'g', 4);
        const double tw = fm.horizontalAdvance(text);
        painter.drawText(QPointF(plotRect.left() - tw - 8, yPix + fm.ascent() / 2.0 - 2), text);
    }
}

void BarPlot::drawCategoryLabels(QPainter& painter, const QRectF& plotRect) const {
    if (m_categories.empty())
        return;

    painter.setPen(QColor(50, 50, 60));
    QFont font = painter.font();
    font.setPointSizeF(std::max(7.0, font.pointSizeF() - 1));
    painter.setFont(font);
    const QFontMetricsF fm(font);

    const int n = static_cast<int>(m_categories.size());
    const double slotW = plotRect.width() / static_cast<double>(n);

    for (int i = 0; i < n; ++i) {
        const double cx = plotRect.left() + (static_cast<double>(i) + 0.5) * slotW;
        const QString& text = m_categories[static_cast<std::size_t>(i)];
        const double tw = fm.horizontalAdvance(text);
        painter.drawText(QPointF(cx - tw / 2.0, plotRect.bottom() + 4 + fm.ascent()), text);
    }
}

void BarPlot::drawAxisTitles(QPainter& painter, const QRectF& plotRect, const QRect& canvasArea) const {
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(QColor(40, 40, 50));
    const QFontMetricsF fm(font);

    painter.drawText(
        QRectF(plotRect.left(), static_cast<double>(canvasArea.bottom() - kMarginBottom + 2),
               plotRect.width(), static_cast<double>(kMarginBottom - 2)),
        static_cast<int>(Qt::AlignHCenter | Qt::AlignTop),
        m_categoryLabel);

    painter.save();
    painter.translate(static_cast<double>(canvasArea.left() + 12),
                      plotRect.center().y());
    painter.rotate(-90);
    painter.drawText(QRectF(-200, -fm.height(), 400, fm.height()), static_cast<int>(Qt::AlignCenter), m_valueLabel);
    painter.restore();
}

void BarPlot::drawBars(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds) const {
    const int n = static_cast<int>(m_categories.size());
    if (n <= 0)
        return;

    const double slotW = plotRect.width() / static_cast<double>(n);
    constexpr double kBarFillRatio = 0.62;
    const double barW = slotW * kBarFillRatio;
    const double yZero = yToPixel(0.0, plotRect, dataBounds);

    painter.setPen(QPen(QColor(52, 88, 158), 1));
    for (int i = 0; i < n; ++i) {
        const double cx = plotRect.left() + (static_cast<double>(i) + 0.5) * slotW;
        const double left = cx - barW / 2.0;
        const double yVal = yToPixel(m_values[static_cast<std::size_t>(i)], plotRect, dataBounds);
        const double top = std::min(yVal, yZero);
        const double height = std::abs(yZero - yVal);
        const QRectF barRect(left, top, barW, height);

        painter.setBrush(QColor(80, 120, 200, 220));
        painter.drawRect(barRect);
    }
}

double BarPlot::yToPixel(double y, const QRectF& plotRect, const Bounds& b) {
    const double dy = b.maxY - b.minY;
    const double ny = dy > 0 ? (y - b.minY) / dy : 0.5;
    return plotRect.bottom() - ny * plotRect.height();
}

std::vector<double> BarPlot::computeTickValues(double minV, double maxV, int maxTicks) {
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

void BarPlot::render(QPainter& painter, const QRect& canvasArea) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    drawCanvasBackground(painter, canvasArea);

    if (m_categories.empty()) {
        drawEmptyState(painter, canvasArea);
        return;
    }

    Bounds b = computeValueBounds();
    b = paddedYBounds(b);
    const QRectF plotRect = plotRectForCanvas(canvasArea);

    drawPlotRegionBackground(painter, plotRect);
    drawAxes(painter, plotRect);

    const std::vector<double> yTicks = computeTickValues(b.minY, b.maxY, 6);
    drawHorizontalGrid(painter, plotRect, b, yTicks);
    drawBars(painter, plotRect, b);
    drawYTickMarksAndLabels(painter, plotRect, b, yTicks);
    drawCategoryLabels(painter, plotRect);
    drawAxisTitles(painter, plotRect, canvasArea);
}
