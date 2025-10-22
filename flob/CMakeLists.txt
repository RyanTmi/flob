add_library(flob SHARED)

set(PUBLIC_HEADERS
    public/containers/hash_map.hpp
    public/containers/map.hpp
    public/containers/vector.hpp

    public/debug/ensure.hpp

    public/log/console.hpp
    public/log/log.hpp

    public/memory/ref.hpp
    public/memory/ref_counted.hpp
    public/misc/uuid.hpp

    public/order_book/order.hpp
    public/order_book/order_book.hpp
    public/order_book/order_feeder.hpp
    public/order_book/order_type.hpp
    public/order_book/session.hpp
    public/order_book/trade.hpp
    public/order_book/types.hpp

    public/core_types.hpp
)

set(PRIVATE_HEADERS
)

set(PRIVATE_SOURCES
    private/log/console.cpp
    private/log/log.cpp

    private/misc/uuid.cpp

    private/order_book/order_book.cpp
)

target_sources(flob
    PUBLIC
        ${PUBLIC_HEADERS}
    PRIVATE
        ${PRIVATE_SOURCES}
        ${PRIVATE_HEADERS}
)

target_include_directories(flob
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/public
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)

set_target_properties(flob PROPERTIES
    OUTPUT_NAME "flob"
    ARCHIVE_OUTPUT_DIRECTORY "${BIN_ROOT}"
    LIBRARY_OUTPUT_DIRECTORY "${BIN_ROOT}"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_ROOT}"
)