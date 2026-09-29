#ifndef WINCCMCP_APP_COMMON_TRANSLATIONMANAGER_H__B6C7D8E9_FAAB_4ABC_9345_E4F5A6B7C8D9__INCLUDED_
#define WINCCMCP_APP_COMMON_TRANSLATIONMANAGER_H__B6C7D8E9_FAAB_4ABC_9345_E4F5A6B7C8D9__INCLUDED_

#include <QObject>
#include <QStringList>
#include <QTranslator>

namespace winccmcp::app::common {

class TranslationManager : public QObject {
    Q_OBJECT

public:
    explicit TranslationManager(QObject* parent = nullptr);

    QStringList availableLanguages() const { return {"en", "ru"}; }
    QString currentLanguage() const { return m_currentLanguage; }

    void switchLanguage(const QString& languageCode);

signals:
    void languageChanged(const QString& languageCode);

private:
    QTranslator m_translator;
    QString m_currentLanguage = QStringLiteral("en");
};

} // namespace winccmcp::app::common

#endif // WINCCMCP_APP_COMMON_TRANSLATIONMANAGER_H__B6C7D8E9_FAAB_4ABC_9345_E4F5A6B7C8D9__INCLUDED_
