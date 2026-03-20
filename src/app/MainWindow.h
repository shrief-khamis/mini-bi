#pragma once

#include <QMainWindow>

class Canvas;
class ControlPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

protected:
    // Override to register menus, actions, and global UI state.
    virtual void configureUi();

private:
    ControlPanel* m_controlPanel = nullptr;
    Canvas* m_canvasPanel = nullptr;
};
