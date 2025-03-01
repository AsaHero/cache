#include <crow.h>
#include <string>
#include "api/handler.hpp"
#include "services/cache.hpp"
#include "services/logger.hpp"

Handler::Handler(std::shared_ptr<Cache<std::string, std::string>> cache)
    : cache_(std::move(cache))
{
    if (!cache_)
    {
        throw std::invalid_argument("Cache pointer cannot be null");
    }
}

crow::response Handler::get(const std::string &key)
{
    auto value = cache_->get(key);
    if (!value)
    {
        Logger::info("Cache miss for key: {}", key);
        return crow::response(404, "Key not found");
    }

    Logger::info("Cache hit for key: {}", key);
    return crow::response(200, *value);
}

crow::response Handler::add(const std::string &key, const std::string &value)
{
    if (key.empty())
    {
        Logger::error("Empty key provided in PUT request");
        return crow::response(400, "Key cannot be empty");
    }

    if (value.empty())
    {
        Logger::error("Empty value provided in PUT request for key: {}", key);
        return crow::response(400, "Value cannot be empty");
    }

    cache_->add(key, value);
    Logger::info("Stored value for key: {}", key);
    return crow::response(200, "Value stored successfully");
}

crow::response Handler::remove(const std::string &key)
{
    if (cache_->remove(key))
    {
        Logger::info("Removed key: {}", key);
        return crow::response(200, "Key removed successfully");
    }

    Logger::info("Key not found for deletion: {}", key);
    return crow::response(404, "Key not found");
}

crow::response Handler::clear()
{
    cache_->clear();
    Logger::info("Cache cleared");
    return crow::response(200, "Cache cleared successfully");
}

crow::response Handler::stats()
{
    crow::json::wvalue stats;
    stats["size"] = cache_->size();
    stats["capacity"] = cache_->capacity();

    Logger::info("Stats requested: size={}, capacity={}",
                 cache_->size(), cache_->capacity());
    return crow::response(200, stats);
}
