#pragma once

class QPainter;
class QRect;

/**
 * Polymorphic plot painter: concrete plot types implement render() for a given canvas rect.
 */
class PlotRenderer {
public:
    virtual ~PlotRenderer() = default;

    virtual void render(QPainter& painter, const QRect& canvasArea) = 0;
};
