#include "app/history/ChatHistoryDialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>

namespace winccmcp::app::history {

using core::models::ChatHistoryRecord;

namespace {

QString formatSources(const core::models::Sources& sources) {
    if (sources.isEmpty()) {
        return ChatHistoryDialog::tr("(no sources)");
    }
    QStringList lines;
    for (const auto& s : sources) {
        lines << QStringLiteral("- %1 (%2)").arg(s.title, s.path);
    }
    return lines.join(QLatin1Char('\n'));
}

} // namespace

ChatHistoryDialog::ChatHistoryDialog(const ChatHistoryRecord& record, QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Query history from %1").arg(record.timestamp.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"))));
    resize(640, 480);

    m_queryLabel = new QLabel(this);
    m_queryLabel->setWordWrap(true);
    m_queryLabel->setText(tr("<b>Query:</b> %1").arg(record.query.toHtmlEscaped()));

    m_answerView = new QTextBrowser(this);
    m_answerView->setPlainText(record.answer);

    m_sourcesLabel = new QLabel(this);
    m_sourcesLabel->setWordWrap(true);
    m_sourcesLabel->setText(tr("<b>Sources:</b><br/>%1")
                .arg(formatSources(record.sources).replace(QLatin1Char('\n'), QStringLiteral("<br/>"))));

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_queryLabel);
    layout->addWidget(m_answerView, 1);
    layout->addWidget(m_sourcesLabel);
    layout->addWidget(buttons);
}

} // namespace winccmcp::app::history
