#pragma once

#include <QWidget>

class PlotRenderer;
class QPaintEvent;

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas() override = default;

    void renderPlaceholderPlot();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void configureUi();
    bool m_showPlaceholderPlot = false;
};
