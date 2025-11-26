#pragma once

#include "llog/types.h"

#include <optional>

namespace llog {

LoggerPtr global();

std::optional<LoggerPtr> logger(const std::string& name);

LoggerPtr loggerOrGlobal(const std::string& name);

void setGlobalLogger(LoggerPtr logger);

}  // namespace llog
