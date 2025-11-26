#pragma once

#include "llog/config.h"

namespace llog {

void loadLogger(const std::string& name, const LoggerConfig& config);

void loadConfig(const Config& config);

}  // namespace llog
