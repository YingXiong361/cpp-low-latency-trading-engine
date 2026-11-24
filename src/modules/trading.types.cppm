module;
#include <cstdint>

export module trading.types;

export namespace trading {

using OrderId = std::uint64_t;
using PriceUnits = std::int64_t;
using Quantity = std::uint64_t;

enum class Side : std::uint8_t { Buy, Sell };

}
