#include "logging/Log.h"

#include <QLoggingCategory>
#include <QString>

#include <QtGlobal>

Q_LOGGING_CATEGORY(uiLog, "minibi.ui")
Q_LOGGING_CATEGORY(appLog, "minibi.app")

void Log::init() {
    // Default Qt message pattern: time, category, level, message
    qSetMessagePattern(QStringLiteral("%{type} %{time yyyy-MM-dd hh:mm:ss.zzz} [%{category}]: %{message}"));

    // Enable our categories; adjust rules as needed (e.g. turn off *.debug in release).
    QLoggingCategory::setFilterRules(QStringLiteral(
        "*.critical=true\n"
        "minibi.ui.debug=true\n"
        "minibi.ui.info=true\n"
        "minibi.app.debug=true\n"
        "minibi.app.info=true\n"));
}

const QLoggingCategory& Log::toQtCategory(LogCategory category) {
    switch (category) {
        case LogCategory::UI: return uiLog();
        case LogCategory::App:return appLog();
    }
    return appLog();
}

void Log::debug(LogCategory category, const QString& message) {
    qCDebug(toQtCategory(category)) << message;
}

void Log::info(LogCategory category, const QString& message) {
    qCInfo(toQtCategory(category)) << message;
}

void Log::warning(LogCategory category, const QString& message) {
    qCWarning(toQtCategory(category)) << message;
}

void Log::critical(LogCategory category, const QString& message) {
    qCCritical(toQtCategory(category)) << message;
}
