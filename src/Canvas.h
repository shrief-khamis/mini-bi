#pragma once

#include <QWidget>

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas() override = default;

private:
    void configureUi();
};
