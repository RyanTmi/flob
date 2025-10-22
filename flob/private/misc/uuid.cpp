#include "misc/uuid.hpp"

#include <random>

namespace flob
{
    static auto random_device        = std::random_device();
    static auto random_engine        = std::mt19937_64(random_device());
    static auto uniform_distribution = std::uniform_int_distribution<uint64>();

    UUID::UUID()
        : _uuid(uniform_distribution(random_engine))
    {}

    UUID::UUID(uint64 uuid)
        : _uuid(uuid)
    {}
}
