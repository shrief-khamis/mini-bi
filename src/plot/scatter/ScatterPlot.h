#pragma once

#include "data/table/DataTable.h"
#include "plot/core/PlotRenderer.h"

#include <QRect>
#include <QRectF>
#include <QString>
#include <vector>

/**
 * Scatter plot: reads numeric pairs from the first two columns of a DataTable,
 * then renders axes, ticks, labels, and points.
 */
class ScatterPlot : public PlotRenderer {
public:
    explicit ScatterPlot(const DataTable& table);
    ~ScatterPlot() override = default;

    void render(QPainter& painter, const QRect& canvasArea) override;

private:
    void setData(const DataTable& table);

    std::vector<double> m_x;
    std::vector<double> m_y;
    QString m_xLabel;
    QString m_yLabel;

    struct Bounds {
        double minX = 0.0;
        double maxX = 1.0;
        double minY = 0.0;
        double maxY = 1.0;
        bool isEmpty = true;
    };

    Bounds computeDataBounds() const;
    static Bounds paddedBounds(const Bounds& b);

    QRectF plotRectForCanvas(const QRect& canvasArea) const;

    void drawCanvasBackground(QPainter& painter, const QRect& canvasArea) const;
    void drawEmptyState(QPainter& painter, const QRect& canvasArea) const;

    void drawPlotRegionBackground(QPainter& painter, const QRectF& plotRect) const;
    void drawAxes(QPainter& painter, const QRectF& plotRect) const;
    void drawGridLines(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                       const std::vector<double>& xTicks, const std::vector<double>& yTicks) const;
    void drawTickMarksAndLabels(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds,
                                const std::vector<double>& xTicks, const std::vector<double>& yTicks) const;
    void drawAxisTitles(QPainter& painter, const QRectF& plotRect, const QRect& canvasArea) const;
    void drawPoints(QPainter& painter, const QRectF& plotRect, const Bounds& dataBounds) const;

    static std::vector<double> computeTickValues(double minV, double maxV, int maxTicks);
    QPointF dataToPixel(double x, double y, const QRectF& plotRect, const Bounds& b) const;
};
