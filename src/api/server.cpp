#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <crow.h>

#include "api/server.hpp"
#include "api/handler.hpp"
#include "services/config.hpp"
#include "services/logger.hpp"

Server::Server(std::shared_ptr<Handler> handler, const Config &config) : handler_(handler), config_(config)
{
    if (!this->handler_)
    {
        throw std::invalid_argument("Handler pointer cannot be null");
    }

    this->routes();
}
void Server::start()
{
    try
    {
        Logger::info("Starting server on {}:{}", config_.host, config_.port);
        this->app_.bindaddr(this->config_.host)
            .loglevel(crow::LogLevel::INFO)
            .port(this->config_.port)
            .multithreaded()
            .concurrency(std::thread::hardware_concurrency())
            .run();
    }
    catch (const std::exception &e)
    {
        Logger::error("Error starting server: {}", e.what());
        throw e;
    }
}

void Server::routes()
{
    CROW_ROUTE(this->app_, "/<string>")
        .methods(crow::HTTPMethod::GET)(
            [this](const std::string &key)
            {
                return this->handler_->get(key);
            });
    CROW_ROUTE(this->app_, "/<string>")
        .methods(crow::HTTPMethod::PUT)(
            [this](const crow::request &req, const std::string &key)
            {
                return this->handler_->add(key, req.body);
            });
    CROW_ROUTE(this->app_, "/<string>")
        .methods(crow::HTTPMethod::DELETE)(
            [this](const std::string &key)
            {
                return this->handler_->remove(key);
            });

    CROW_ROUTE(this->app_, "/")
        .methods(crow::HTTPMethod::DELETE)(
            [this]()
            {
                return this->handler_->clear();
            });
    CROW_ROUTE(this->app_, "/cache/stats")
        .methods(crow::HTTPMethod::GET)(
            [this]()
            {
                return this->handler_->stats();
            });
}