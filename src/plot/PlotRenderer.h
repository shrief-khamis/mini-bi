#pragma once

class QPainter;
class QRect;

class PlotRenderer {
public:
    PlotRenderer() = default;
    ~PlotRenderer() = default;

    // Draw a placeholder centered circle inside the given canvas area.
    void drawCenteredCircle(QPainter& painter, const QRect& canvasArea) const;
};
