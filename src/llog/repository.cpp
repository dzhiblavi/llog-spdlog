#include "llog/repository.h"

#include <spdlog/spdlog.h>

namespace llog {

namespace {

class GlobalLogger {
 public:
    static GlobalLogger& instance() {
        static GlobalLogger instance;
        return instance;
    }

    LoggerPtr get() { return logger; }

    void set(LoggerPtr instance) { logger = std::move(instance); }

 private:
    LoggerPtr logger = spdlog::default_logger();
};

}  // namespace

std::optional<LoggerPtr> logger(const std::string& name) {
    if (auto logger = spdlog::get(name)) {
        return logger;
    }

    return std::nullopt;
}

LoggerPtr loggerOrGlobal(const std::string& name) {
    return logger(name).value_or(global());
}

void setGlobalLogger(std::shared_ptr<spdlog::logger> logger) {
    GlobalLogger::instance().set(std::move(logger));
}

LoggerPtr global() {
    return GlobalLogger::instance().get();
}

}  // namespace llog
