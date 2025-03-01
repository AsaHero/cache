#pragma once

#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <crow.h>

#include "api/handler.hpp"
#include "services/config.hpp"

class Server
{
private:
    crow::SimpleApp app_;
    std::shared_ptr<Handler> handler_;
    Config config_;

    void routes();

public:
    explicit Server(std::shared_ptr<Handler> handler, const Config &config);
    ~Server() = default;

    // Delete copy constructor and assignment operator
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;

    // Move constructor and assignment operator
    Server(Server &&) = default;
    Server &operator=(Server &&) = default;

    void start();
};