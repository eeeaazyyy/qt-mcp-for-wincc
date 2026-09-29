#ifndef WINCCMCP_CORE_MODELS_RAGANSWER_H__5E6F7081_9203_4415_C637_E8F9A0B1C2D3__INCLUDED_
#define WINCCMCP_CORE_MODELS_RAGANSWER_H__5E6F7081_9203_4415_C637_E8F9A0B1C2D3__INCLUDED_

#include <QString>

#include "core/models/Source.h"

namespace winccmcp::core::models {

// Result of RagService::answer() - what both the GUI and the JSON-RPC client see.
struct RagAnswer {
    QString answer;
    Sources sources;
};

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_RAGANSWER_H__5E6F7081_9203_4415_C637_E8F9A0B1C2D3__INCLUDED_
