#include "infrastructure/persistence/SqliteEnvironment.h"

#include <mutex>

#include <sqlite3.h>

#include "sqlite-vec.h"

namespace winccmcp::infrastructure::persistence {

void SqliteEnvironment::initializeOnce() {
    static std::once_flag flag;
    std::call_once(flag, [] {
        // Casting the function's signature to void(*)(void) - the documented
        // way to register auto-loaded extensions, per sqlite3.h itself.
        sqlite3_auto_extension(reinterpret_cast<void (*)()>(sqlite3_vec_init));
    });
}

} // namespace winccmcp::infrastructure::persistence
