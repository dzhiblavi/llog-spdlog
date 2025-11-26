#pragma once

#include <spdlog/logger.h>

#include <memory>

namespace llog {

using Logger = spdlog::logger;
using LoggerPtr = std::shared_ptr<Logger>;

enum class Level : std::underlying_type_t<spdlog::level::level_enum> {
    Trace = spdlog::level::trace,
    Debug = spdlog::level::debug,
    Info = spdlog::level::info,
    Warn = spdlog::level::warn,
    Err = spdlog::level::err,
    Critical = spdlog::level::critical,
    Off = spdlog::level::off,
};

}  // namespace llog
