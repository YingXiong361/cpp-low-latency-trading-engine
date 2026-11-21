module;
#include <cstdint>

export module trading.instrument;

export namespace trading {

struct Instrument {
    std::uint32_t id{};
};

}
