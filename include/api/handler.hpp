#pragma once

#include <crow.h>
#include "services/cache.hpp"

class Handler
{
private:
    std::shared_ptr<Cache<std::string, std::string>> cache_;

public:
    explicit Handler(std::shared_ptr<Cache<std::string, std::string>> cache);
    crow::response get(const std::string &key);
    crow::response add(const std::string &key, const std::string &value);
    crow::response remove(const std::string &key);
    crow::response clear();
    crow::response stats();
};