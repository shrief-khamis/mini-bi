#pragma once

#include "plot/core/PlotRenderer.h"

/**
 * Status message on the canvas (no chart): whether data has been confirmed after import.
 */
class PlaceholderPlot : public PlotRenderer {
public:
    explicit PlaceholderPlot(bool dataLoaded);
    ~PlaceholderPlot() override = default;

    void render(QPainter& painter, const QRect& canvasArea) override;

private:
    void drawStatusText(QPainter& painter, const QRect& canvasArea, bool dataLoaded) const;

    bool m_dataLoaded = false;
};
