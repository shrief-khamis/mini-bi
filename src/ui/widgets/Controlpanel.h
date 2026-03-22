#pragma once

#include <QWidget>

class QPushButton;

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel() override = default;

signals:
    void loadDataRequested();
    void plotRequested();
    void wipeRequested();

private:
    void configureUi();
    void wireSignals();
    void onLoadDataButtonClicked();
    void onPlotButtonClicked();
    void onWipeButtonClicked();

    QPushButton* m_loadDataButton = nullptr;
    QPushButton* m_plotButton = nullptr;
    QPushButton* m_wipeButton = nullptr;
};
