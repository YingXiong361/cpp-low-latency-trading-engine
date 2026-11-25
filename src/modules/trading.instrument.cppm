export module trading.instrument;
export import trading.types;

export namespace trading {

struct Instrument {
    InstrumentId id;

    constexpr Instrument(InstrumentId instrument_id) noexcept : id{instrument_id} {}
};

}
