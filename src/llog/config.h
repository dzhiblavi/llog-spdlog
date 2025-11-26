#pragma once

#include "llog/types.h"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <map>
#include <string>

namespace llog {

enum class SyncMode {
    Sync,
    AsyncBlocking,
    AsyncOverflow,
};

enum class SinkType {
    Stdout,
    Stderr,
    File,
    RotatingFile,
    DailyFile,
};

struct SinkConfig {
    static constexpr size_t DefaultMaxSize = 1ULL << 20ULL;
    static constexpr size_t DefaultMaxNumFiles = 5;

    Level level = Level::Trace;
    SinkType type = SinkType::Stdout;
    bool colors = false;
    std::string path;
    uint16_t hour;
    uint16_t minute;

    size_t max_size = DefaultMaxSize;
    size_t max_num_files = DefaultMaxNumFiles;
};

struct LoggerConfig {
    Level level = Level::Info;
    std::string format = "";
    SyncMode sync_mode = SyncMode::Sync;
    bool force_flush = true;
    std::vector<SinkConfig> sinks;
};

struct ThreadPoolConfig {
    static constexpr size_t DefaultQueueSize = 8192;

    size_t queue_size = DefaultQueueSize;
    size_t num_threads = 1;
};

struct Config {
    ThreadPoolConfig thread_pool = {};
    std::map<std::string, LoggerConfig> loggers;
};

}  // namespace llog
