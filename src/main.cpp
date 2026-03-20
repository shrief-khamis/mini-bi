#include "app/MainWindow.h"
#include "logging/Log.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Log::init();
    Log::info(LogCategory::App, QStringLiteral("Application starting"));

    MainWindow window;
    window.show();

    return app.exec();
}
