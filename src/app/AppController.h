#pragma once

#include <QObject>

class MainWindow;

class AppController : public QObject {
    Q_OBJECT

public:
    explicit AppController(MainWindow* mainWindow, QObject* parent = nullptr);
    ~AppController() override = default;

private slots:
    void onLoadDataRequested();
    void onPlotRequested();

private:
    void wireSignals();

    MainWindow* m_mainWindow = nullptr;
};
