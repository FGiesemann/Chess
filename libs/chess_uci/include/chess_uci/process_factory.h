/* ************************************************************************** *
 * Chess UCI                                                                  *
 * Universal Chess Interface for Chess Engines                                *
 * ************************************************************************** */

#ifndef CHESS_UCI_PROCESS_FACTORY_H
#define CHESS_UCI_PROCESS_FACTORY_H

#include "chess_uci/engine_process.h"

#include <memory>

namespace chess_uci {

class ProcessFactory {
public:
    static auto create_local() -> std::unique_ptr<EngineProcess>;
};

} // namespace chess_uci

#endif
