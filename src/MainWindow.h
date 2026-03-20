#pragma once

#include <QMainWindow>

class Canvas;
class Controlpanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    // Override to register menus, actions, and global UI state.
    virtual void configureUi();

private:
    Controlpanel* m_controlPanel = nullptr;
    Canvas* m_canvasPanel = nullptr;
};
