#pragma once

#include "llog/repository.h"
#include "llog/types.h"

namespace llog {

class WithLogger {
 public:
    WithLogger() = default;

    void setLogger(LoggerPtr logger) { logger_ = std::move(logger); }

    LoggerPtr getLogger() const { return logger_; }

 private:
    LoggerPtr logger_ = global();
};

}  // namespace llog
