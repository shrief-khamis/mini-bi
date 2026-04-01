#pragma once

#include "data/table/DataTable.h"
#include "plot/core/PlotRenderer.h"

#include <QRect>
#include <QRectF>
#include <QString>
#include <vector>

/**
 * Vertical bar chart: first column is category labels, second column numeric values.
 * Rows are drawn in table order.
 */
class BarPlot : public PlotRenderer {
public:
    explicit BarPlot(const DataTable& table);
    ~BarPlot() override = default;

    void render(QPainter& painter, const QRect& canvasArea) override;

private:
    void setData(const DataTable& table);

    std::vector<QString> m_categories;
    std::vector<double> m_values;
    QString m_categoryLabel;
    QString m_valueLabel;

    struct Bounds {
        double minY = 0.0;
        double maxY = 1.0;
        bool isEmpty = true;
    };

    Bounds computeValueBounds() const;
    static Bounds paddedYBounds(const Bounds& b);

    QRectF plotRectForCanvas(const QRect& canvasArea) const;

    void drawCanvasBackground(QPainter& painter, const QRect& canvasArea) const;
    void drawEmptyState(QPainter& painter, const QRect& canvasArea) const;

    void drawPlotRegionBackground(QPainter& painter, const QRectF& plotRect) const;
    void drawAxes(QPainter& painter, const QRectF& plotRect) const;
    void drawHorizontalGrid(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                            const std::vector<double>& yTicks) const;
    void drawYTickMarksAndLabels(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                                 const std::vector<double>& yTicks) const;
    void drawCategoryLabels(QPainter& painter, const QRectF& plotRect) const;
    void drawAxisTitles(QPainter& painter, const QRectF& plotRect, const QRect& canvasArea) const;
    void drawBars(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds) const;

    static std::vector<double> computeTickValues(double minV, double maxV, int maxTicks);
    static double yToPixel(double y, const QRectF& plotRect, const Bounds& b);
};
