module;
#include <cstdint>
#include <stdexcept>

export module trading.types;

export namespace trading {
class OrderId {
public:
    explicit constexpr OrderId(std::uint64_t value) : value_{value} {
        if (value == 0) {
            throw std::invalid_argument("OrderId must be nonzero");
        }
    }

    [[nodiscard]] constexpr std::uint64_t value() const noexcept { return value_; }
    constexpr bool operator==(const OrderId&) const noexcept = default;

private:
    std::uint64_t value_;
};

class InstrumentId {
public:
    explicit constexpr InstrumentId(std::uint64_t value) : value_{value} {
        if (value == 0) {
            throw std::invalid_argument("InstrumentId must be nonzero");
        }
    }

    [[nodiscard]] constexpr std::uint64_t value() const noexcept { return value_; }
    constexpr bool operator==(const InstrumentId&) const noexcept = default;

private:
    std::uint64_t value_;
};

struct Quantity {
    std::uint64_t value{};
    constexpr auto operator<=>(const Quantity&) const noexcept = default;

    constexpr Quantity operator+(const Quantity& other) const {
        return Quantity(value + other.value);
    }

    constexpr Quantity operator-(const Quantity& other) const {
        return Quantity(value - other.value);
    }
};

struct PriceUnits {
    std::int64_t value{};
    constexpr auto operator<=>(const PriceUnits&) const noexcept = default;

    constexpr PriceUnits operator+(const PriceUnits& other) const {
        return PriceUnits(value + other.value);
    }

    constexpr PriceUnits operator-(const PriceUnits& other) const {
        return PriceUnits(value - other.value);
    }
};

enum class Side : std::uint8_t { Buy, Sell };

}
