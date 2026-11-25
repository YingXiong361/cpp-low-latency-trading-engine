export module trading.order;
export import trading.types;
export import trading.instrument;

export namespace trading {

struct Order {
    OrderId id;
    Instrument instrument;
    Side side{};
    PriceUnits price{};
    Quantity quantity{};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return (side == Side::Buy || side == Side::Sell)
            && price > PriceUnits{0}
            && quantity > Quantity{0};
    }
};

}
