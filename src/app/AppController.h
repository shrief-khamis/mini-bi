#pragma once

#include "data/table/DataTable.h"

#include <QObject>

#include <optional>

class MainWindow;

class AppController : public QObject {
    Q_OBJECT

public:
    explicit AppController(MainWindow* mainWindow, QObject* parent = nullptr);
    ~AppController() override = default;

private slots:
    void onLoadDataRequested();
    void onPlotRequested();
    void onWipeRequested();
    void onImportPreviewCanceled();

private:
    void wireSignals();
    void applyConfirmedImport(const DataTable& table);

    MainWindow* m_mainWindow = nullptr;
    std::optional<DataTable> m_loadedData;
};
