export module trading.order;
export import trading.types;
export import trading.instrument;

export namespace trading {

struct Order {
    OrderId id{};
    Instrument instrument{};
    Side side{};
    PriceTicks price{};
    Quantity quantity{};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return id != 0 && instrument.id != 0 && price > 0 && quantity != 0
            && (side == Side::Buy || side == Side::Sell);
    }
};

}
