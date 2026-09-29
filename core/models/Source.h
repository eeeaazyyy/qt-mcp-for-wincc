#ifndef WINCCMCP_CORE_MODELS_SOURCE_H__4D5E6F70_8192_4304_B526_D7E8F9A0B1C2__INCLUDED_
#define WINCCMCP_CORE_MODELS_SOURCE_H__4D5E6F70_8192_4304_B526_D7E8F9A0B1C2__INCLUDED_

#include <QString>
#include <QVector>

namespace winccmcp::core::models {

// A reference to the source document the generated answer relies on.
struct Source {
    QString path;
    QString title;
    double distance = 0.0;
};

using Sources = QVector<Source>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_SOURCE_H__4D5E6F70_8192_4304_B526_D7E8F9A0B1C2__INCLUDED_
