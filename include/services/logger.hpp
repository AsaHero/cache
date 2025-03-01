#pragma once

#include <fmt/format.h>
#include <fmt/chrono.h>
#include <iostream>
#include <chrono>

class Logger
{
public:
    template <typename... Args>
    static void info(fmt::format_string<Args...> fmt, Args &&...args)
    {
        log("INFO", fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void error(fmt::format_string<Args...> fmt, Args &&...args)
    {
        log("ERROR", fmt, std::forward<Args>(args)...);
    }

private:
    template <typename... Args>
    static void log(const char *level, fmt::format_string<Args...> fmt, Args &&...args)
    {
        auto now = std::chrono::system_clock::now();
        std::cout << fmt::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}\n", now, level, fmt::format(fmt, std::forward<Args>(args)...));
    }
};