#include "api/handler.hpp"
#include "api/server.hpp"
#include "services/cache.hpp"
#include "services/config.hpp"
#include "services/logger.hpp"

int main()
{
    try
    {
        auto config = Config::New();
        auto cache = std::make_shared<Cache<std::string, std::string>>(config.cache_max_size);
        auto handler = std::make_shared<Handler>(cache);
        auto server = std::make_unique<Server>(handler, config);

        server->start();
    }
    catch (const std::exception &e)
    {
        Logger::error("Failed to run application: {}", e.what());
        return 1;
    }
}