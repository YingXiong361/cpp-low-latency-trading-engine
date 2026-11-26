module;
#include <cstdint>
#include <stdexcept>

export module trading.types;

export namespace trading {
class OrderId {
public:
    explicit constexpr OrderId(std::int64_t value) : value_{value} {
        if (value <= 0) {
            throw std::invalid_argument("OrderId must be positive");
        }
    }

    [[nodiscard]] constexpr std::int64_t value() const noexcept { return value_; }
    constexpr bool operator==(const OrderId&) const noexcept = default;

private:
    std::int64_t value_;
};

class InstrumentId {
public:
    explicit constexpr InstrumentId(std::int64_t value) : value_{value} {
        if (value <= 0) {
            throw std::invalid_argument("InstrumentId must be positive");
        }
    }

    [[nodiscard]] constexpr std::int64_t value() const noexcept { return value_; }
    constexpr bool operator==(const InstrumentId&) const noexcept = default;

private:
    std::int64_t value_;
};

class Quantity {
public:
    constexpr Quantity() noexcept = default;

    explicit constexpr Quantity(std::int64_t value) : value_{value} {
        if (value < 0) {
            throw std::invalid_argument("Quantity must be nonnegative");
        }
    }

    [[nodiscard]] constexpr std::int64_t value() const noexcept { return value_; }
    constexpr auto operator<=>(const Quantity&) const noexcept = default;

    constexpr Quantity operator+(const Quantity& other) const {
        Quantity result{};
        if (__builtin_add_overflow(value_, other.value_, &result.value_)) {
            throw std::overflow_error("Quantity addition out of range");
        }
        return result;
    }

    constexpr Quantity operator-(const Quantity& other) const {
        if (*this < other) {
            throw std::overflow_error("Quantity subtraction out of range");
        }
        return Quantity{value_ - other.value_};
    }

private:
    std::int64_t value_{};
};

struct PriceUnits {
    std::int64_t value{};
    constexpr auto operator<=>(const PriceUnits&) const noexcept = default;

    constexpr PriceUnits operator+(const PriceUnits& other) const {
        PriceUnits result{};
        if (__builtin_add_overflow(value, other.value, &result.value)) {
            throw std::overflow_error("PriceUnits addition out of range");
        }
        return result;
    }

    constexpr PriceUnits operator-(const PriceUnits& other) const {
        PriceUnits result{};
        if (__builtin_sub_overflow(value, other.value, &result.value)) {
            throw std::overflow_error("PriceUnits subtraction out of range");
        }
        return result;
    }
};

enum class Side : std::uint8_t { Buy, Sell, Unspecified };

}
