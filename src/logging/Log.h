#pragma once

#include <QString>

class QLoggingCategory;

enum class LogCategory {
    UI,
    App,
};

class Log {
public:
    /** Call once after QCoreApplication exists (e.g. right after QApplication construction). */
    static void init();

    static const QLoggingCategory& toQtCategory(LogCategory category);

    static void debug(LogCategory category, const QString& message);
    static void info(LogCategory category, const QString& message);
    static void warning(LogCategory category, const QString& message);
    static void critical(LogCategory category, const QString& message);

private:
    Log() = delete;
};
