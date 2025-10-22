add_executable(example)

set(PUBLIC_HEADERS
    
)

set(PRIVATE_SOURCES
    private/main.cpp
)

target_sources(example
    PUBLIC
        ${PUBLIC_HEADERS}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_include_directories(example
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(example
    PRIVATE
        flob
)

set_target_properties(example PROPERTIES
    OUTPUT_NAME "example"
    ARCHIVE_OUTPUT_DIRECTORY "${BIN_ROOT}"
    LIBRARY_OUTPUT_DIRECTORY "${BIN_ROOT}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_ROOT}"
)
