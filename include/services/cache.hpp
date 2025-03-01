#pragma once

#include <optional>
#include <mutex>
#include <unordered_map>
#include <list>
#include <stdexcept>

template <typename Key, typename Value>
class Cache
{
private:
    const size_t capacity_;
    mutable std::mutex mutex_;

    using Iterator = typename std::list<std::pair<Key, Value>>::iterator;
    using LRUCache = std::unordered_map<Key, Iterator>;

    // double linked list to store the access history
    std::list<std::pair<Key, Value>> accessList_;

    // O(1) cache
    LRUCache cache_;

    void evict()
    {
        if (this->cache_.size() >= this->capacity_)
        {
            const auto &last = this->accessList_.back();
            this->cache_.erase(last.first);
            this->accessList_.pop_back();
        }
    }

    void reorder(const Iterator &it)
    {
        this->accessList_.splice(this->accessList_.begin(), this->accessList_, it);
    }

public:
    explicit Cache(size_t capacity) : capacity_(capacity)
    {
        if (capacity == 0)
        {
            throw std::invalid_argument("Cache capacity must be greater than 0");
        }
    }

    ~Cache() = default;

    // delete copy constructor and copy assignment
    Cache(const Cache &) = delete;
    Cache &operator=(const Cache &) = delete;

    // move constructor and move assignment
    Cache(Cache &&) noexcept = default;
    Cache &operator=(Cache &&) noexcept = default;

    std::optional<Value> get(const Key &key)
    {
        std::lock_guard<std::mutex> lock(this->mutex_);

        auto it = this->cache_.find(key);
        if (it == this->cache_.end())
        {
            return std::nullopt;
        }

        this->reorder(it->second);
        return it->second->second;
    }

    void add(const Key &key, Value value)
    {
        std::lock_guard<std::mutex> lock(this->mutex_);

        auto it = this->cache_.find(key);
        if (it != this->cache_.end())
        {
            it->second->second = std::move(value);
            this->reorder(it->second);
            return;
        }

        this->evict();

        this->accessList_.emplace_front(key, std::move(value));
        this->cache_.emplace(key, this->accessList_.begin());
    }

    bool remove(const Key &key)
    {
        std::lock_guard<std::mutex> lock(this->mutex_);

        auto it = this->cache_.find(key);
        if (it == this->cache_.end())
        {
            return false;
        }

        this->accessList_.erase(it->second);
        this->cache_.erase(it);
        return true;
    }

    void clear()
    {
        std::lock_guard<std::mutex> lock(this->mutex_);
        this->accessList_.clear();
        this->cache_.clear();
    }

    bool contains(const Key &key)
    {
        std::lock_guard<std::mutex> lock(this->mutex_);
        return this->cache_.find(key) != this->cache_.end();
    }

    [[nodiscard]] size_t size() const
    {
        std::lock_guard<std::mutex> lock(this->mutex_);
        return this->cache_.size();
    }

    [[nodiscard]] size_t capacity() const
    {
        return this->capacity_;
    }
};