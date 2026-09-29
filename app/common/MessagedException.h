#ifndef WINCCMCP_APP_COMMON_MESSAGEDEXCEPTION_H__1F2E3D4C_5B6A_4978_8877_665544332211__INCLUDED_
#define WINCCMCP_APP_COMMON_MESSAGEDEXCEPTION_H__1F2E3D4C_5B6A_4978_8877_665544332211__INCLUDED_

#include <QByteArray>
#include <QException>
#include <QString>

namespace winccmcp::app::common {

class MessagedException : public QException {
public:
    explicit MessagedException(const QString& message) : m_message(message.toUtf8()) {}

    const char* what() const noexcept override { return m_message.constData(); }
    void raise() const override { throw *this; }
    MessagedException* clone() const override { return new MessagedException(*this); }

private:
    QByteArray m_message;
};

} // namespace winccmcp::app::common

#endif // WINCCMCP_APP_COMMON_MESSAGEDEXCEPTION_H__1F2E3D4C_5B6A_4978_8877_665544332211__INCLUDED_
