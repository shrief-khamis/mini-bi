#pragma once

#include "plot/core/PlotRenderer.h"

#include <QWidget>

#include <memory>

class QPaintEvent;

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas() override = default;

    /** Takes ownership of the renderer and schedules a repaint. */
    void renderPlot(std::unique_ptr<PlotRenderer> renderer);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void configureUi();

    std::unique_ptr<PlotRenderer> m_renderer;
};
