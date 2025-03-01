add_executable(${PROJECT_NAME}
    main.cpp
    api/handler.cpp
    api/server.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Crow::Crow
        fmt::fmt
)


target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include

    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)