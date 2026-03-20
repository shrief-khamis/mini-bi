#pragma once

#include <QWidget>

class QPushButton;

class Controlpanel : public QWidget {
    Q_OBJECT

public:
    explicit Controlpanel(QWidget* parent = nullptr);
    ~Controlpanel() override = default;

signals:
    void loadDataRequested();
    void plotRequested();

private:
    void configureUi();
    void wireSignals();

    QPushButton* m_loadDataButton = nullptr;
    QPushButton* m_plotButton = nullptr;
};
