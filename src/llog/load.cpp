#include "llog/load.h"
#include "llog/repository.h"

#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <filesystem>

namespace llog {

namespace {

std::filesystem::path getHomeDirectory() {
    auto* ptr = ::getenv("HOME");
    assert(ptr);
    return ptr;
}

std::filesystem::path expandHome(const std::filesystem::path& path) {
    std::string str = path;

    if (str.starts_with('~')) {
        str.replace(0, 1, getHomeDirectory());
    }

    return str;
}

std::string absolute(const std::string& path) {
    return expandHome(path);
}

constexpr auto GlobalLoggerName = "global";

spdlog::sink_ptr loadSink(const SinkConfig& s) {
    auto sink = [&] -> spdlog::sink_ptr {
        switch (s.type) {
            using enum SinkType;

            case Stdout:
                if (s.colors) {
                    return std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                }
                return std::make_shared<spdlog::sinks::stdout_sink_mt>();

            case Stderr:
                if (s.colors) {
                    return std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
                }
                return std::make_shared<spdlog::sinks::stderr_sink_mt>();

            case File:
                return std::make_shared<spdlog::sinks::basic_file_sink_mt>(absolute(s.path));

            case RotatingFile:
                return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    absolute(s.path), s.max_size, s.max_num_files);

            case DailyFile:
                return std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                    absolute(s.path), s.hour, s.minute);
        }
    }();

    sink->set_level(static_cast<spdlog::level::level_enum>(s.level));
    return sink;
}

}  // namespace

void loadLogger(const std::string& name, const LoggerConfig& config) {
    std::vector<spdlog::sink_ptr> sinks;
    sinks.reserve(config.sinks.size());
    std::transform(config.sinks.begin(), config.sinks.end(), std::back_inserter(sinks), loadSink);

    auto logger = [&]() -> std::shared_ptr<spdlog::logger> {
        switch (config.sync_mode) {
            using enum SyncMode;
            using enum spdlog::async_overflow_policy;

            case Sync:
                return std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
            case AsyncBlocking:
            case AsyncOverflow:
                return std::make_shared<spdlog::async_logger>(
                    name,
                    sinks.begin(),
                    sinks.end(),
                    spdlog::thread_pool(),
                    config.sync_mode == AsyncBlocking ? block : overrun_oldest);
        }

        return {};
    }();

    logger->set_level(static_cast<spdlog::level::level_enum>(config.level));

    if (!config.format.empty()) {
        logger->set_pattern(config.format);
    }

    if (config.force_flush) {
        logger->flush_on(spdlog::level::trace);
    }

    spdlog::register_logger(logger);
}

void loadConfig(const Config& config) {
    spdlog::init_thread_pool(config.thread_pool.queue_size, config.thread_pool.num_threads);

    for (const auto& [name, conf] : config.loggers) {
        loadLogger(name, conf);
    }

    if (auto global = spdlog::get(GlobalLoggerName)) {
        setGlobalLogger(global);
    }
}

}  // namespace llog
