#pragma once

#include <string>
#include <cstdlib>
#include <stdexcept>

inline std::string getEnvOrDefault(const char *name, const char *defaultValue)
{
    const char *value = std::getenv(name);
    return value ? value : defaultValue;
}

inline int getEnvIntOrDefault(const char *name, int defaultValue)
{
    const char *value = std::getenv(name);
    if (value)
    {
        try
        {
            return std::stoi(value);
        }
        catch (const std::invalid_argument &)
        {
            throw std::runtime_error(std::string("Invalid integer for environment variable: ") + name);
        }
    }
    return defaultValue;
}

inline size_t getEnvSizeOrDefault(const char *name, size_t defaultValue)
{
    const char *value = std::getenv(name);
    if (value)
    {
        try
        {
            return static_cast<size_t>(std::stoull(value));
        }
        catch (const std::invalid_argument &)
        {
            throw std::runtime_error(std::string("Invalid size for environment variable: ") + name);
        }
    }
    return defaultValue;
}

struct Config
{
    std::string host;
    int port;
    size_t cache_max_size;

    static Config New()
    {
        Config config;
        config.host = getEnvOrDefault("CACHE_HOST", "0.0.0.0");
        config.port = getEnvIntOrDefault("CACHE_PORT", 8080);
        config.cache_max_size = getEnvSizeOrDefault("CACHE_MAX_SIZE", 100);
        return config;
    }
};
