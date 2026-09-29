#include "app/common/TranslationManager.h"

#include <QCoreApplication>
#include <QDir>

namespace winccmcp::app::common {

TranslationManager::TranslationManager(QObject* parent) : QObject(parent) {
}

void TranslationManager::switchLanguage(const QString& languageCode) {
    QCoreApplication::removeTranslator(&m_translator);

    if (languageCode != QStringLiteral("en")) {
        const QString qmPath = QDir(QCoreApplication::applicationDirPath())
                                    .filePath(QStringLiteral("i18n/winccmcp_%1.qm").arg(languageCode));
        if (m_translator.load(qmPath)) {
            QCoreApplication::installTranslator(&m_translator);
        }
    }

    m_currentLanguage = languageCode;
    emit languageChanged(languageCode);
}

} // namespace winccmcp::app::common
