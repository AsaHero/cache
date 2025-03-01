# Cache API

A high-performance in-memory key-value cache with a RESTful API interface built with C++ and [Crow](https://github.com/CrowCpp/Crow) framework.

## Overview

This project implements a thread-safe, LRU (Least Recently Used) caching service accessible via a REST API. The cache supports basic operations such as storing, retrieving, and deleting key-value pairs, as well as cache statistics and management.

## Features

- **Thread-safe in-memory LRU cache**
  - Fixed-size cache with automatic eviction
  - O(1) lookup complexity
  - Thread safety with mutex locking

- **RESTful API**
  - Get, add, and remove individual key-value pairs
  - Clear entire cache
  - Get cache statistics

- **Performance optimized**
  - Multithreaded request handling
  - Automatic thread scaling based on system resources

## Technical Details

### Cache Implementation

The cache is implemented as a generic templated class that combines:
- A doubly-linked list to track access order (most recently used items at the front)
- An unordered map (hash table) for O(1) lookups
- Mutex locking for thread safety

When the cache reaches capacity, the least recently used item is automatically evicted to make room for new entries.

### API Endpoints

| Method | Endpoint       | Description                                  |
|--------|----------------|----------------------------------------------|
| GET    | `/<key>`       | Retrieves the value for the specified key    |
| PUT    | `/<key>`       | Stores a value with the specified key        |
| DELETE | `/<key>`       | Removes the specified key and its value      |
| DELETE | `/`            | Clears the entire cache                      |
| GET    | `/cache/stats` | Returns statistics about the cache           |

### Response Formats

- **GET `/<key>`**
  - Success (200): Value as plain text
  - Not Found (404): "Key not found"

- **PUT `/<key>`**
  - Success (200): "Value stored successfully"
  - Bad Request (400): Error message if key or value is empty

- **DELETE `/<key>`**
  - Success (200): "Key removed successfully"
  - Not Found (404): "Key not found"

- **DELETE `/`**
  - Success (200): "Cache cleared successfully"

- **GET `/cache/stats`**
  - Success (200): JSON with cache size and capacity
  ```json
  {
    "size": 10,
    "capacity": 100
  }
  ```

## Project Structure

```
.
├── CMakeLists.txt           # Main CMake configuration
├── include/                 # Header files
│   ├── api/
│   │   ├── handler.hpp      # API endpoint handlers
│   │   └── server.hpp       # HTTP server setup
│   └── services/
│       ├── cache.hpp        # LRU cache implementation
│       ├── config.hpp       # Application configuration
│       └── logger.hpp       # Logging service
└── src/                     # Source files
    ├── CMakeLists.txt       # Source CMake configuration
    ├── main.cpp             # Application entry point
    └── api/
        ├── handler.cpp      # API endpoint handler implementation
        └── server.cpp       # Server implementation
```

## Dependencies

- [Crow](https://github.com/CrowCpp/Crow) - C++ microframework for web
- [fmt](https://github.com/fmtlib/fmt) - Modern formatting library

## Building

### Prerequisites

- CMake 3.20 or higher
- C++17 compatible compiler
- Crow library
- fmt library

### Compilation

```bash
mkdir build && cd build
cmake ..
make
```

## Running

After building, you can run the application:

```bash
./cache
```

By default, the server will listen on `0.0.0.0:8080`, but this can be configured via the `Config` class.

## Usage Examples

### Store a value

```bash
curl -X PUT -d "Hello, world!" http://localhost:8080/greeting
```

### Retrieve a value

```bash
curl http://localhost:8080/greeting
```

### Delete a value

```bash
curl -X DELETE http://localhost:8080/greeting
```

### View cache statistics

```bash
curl http://localhost:8080/cache/stats
```

## Configuration

The application is configured through environment variables parsed by the `Config` class. The configuration includes:

- Server host address
- Server port
- Cache maximum size

Example environment variables that can be set:
```bash
# Example (adjust based on your actual implementation)
CACHE_HOST=0.0.0.0
CACHE_PORT=8080
CACHE_MAX_SIZE=1000
```

The `Config::New()` method parses these environment variables and provides default values when variables are not set.
