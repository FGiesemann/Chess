/* ************************************************************************** *
 * Chess UCI                                                                  *
 * Universal Chess Interface for Chess Engines                                *
 * ************************************************************************** */

#include "chess_uci/process_factory.h"

#if defined(CHESS_UCI_WINDOWS)
#include "chess_uci/engine_process_win.h"
using LocalEngineProcess = chess_uci::EngineProcessWin;
#elif defined(CHESS_UCI_UNIX)
#include "chess_uci/engine_process_unix.h"
using LocalEngineProcess = chess_uci::EngineProcessUnix;
#else
#error "Platform not configured in CMake"
#endif

namespace chess_uci {

auto ProcessFactory::create_local() -> std::unique_ptr<EngineProcess> {
    return std::make_unique<LocalEngineProcess>();
}

} // namespace chess_uci
