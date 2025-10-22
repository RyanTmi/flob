add_executable(sandbox)

set(PUBLIC_HEADERS
    
)

set(PRIVATE_SOURCES
    private/main.cpp
)

target_sources(sandbox
    PUBLIC
        ${PUBLIC_HEADERS}
    PRIVATE
        ${PRIVATE_SOURCES}
)

target_include_directories(sandbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_libraries(sandbox
    PRIVATE
        flob
)

set_target_properties(sandbox PROPERTIES
    OUTPUT_NAME "sandbox"
    ARCHIVE_OUTPUT_DIRECTORY "${BIN_ROOT}"
    LIBRARY_OUTPUT_DIRECTORY "${BIN_ROOT}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_ROOT}"
)
