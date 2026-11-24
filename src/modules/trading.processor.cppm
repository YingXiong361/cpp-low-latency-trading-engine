module;

#include <unordered_map>
#include <cstdint>
#include <string_view>

export module trading.processor;
export import trading.order;

export namespace trading {

enum class SubmitResult: std::uint8_t { Accepted, Rejected };

enum class SubmitRejectReason {
    InvalidOrder,
    DuplicateId,
    None
};

constexpr std::string_view reason_name(trading::SubmitRejectReason reason) noexcept {
    using enum trading::SubmitRejectReason;
    switch (reason) {
    case None:         return "None";
    case InvalidOrder: return "InvalidOrder";
    case DuplicateId:  return "DuplicateId";
    }
    return "Unknown";
}

struct SubmitResponse {
    SubmitResult Result {};
    SubmitRejectReason RejectReason{SubmitRejectReason::None};
};

class OrderProcessor {
public:
    [[nodiscard]] SubmitResponse submit(const Order& order);

private:
    std::unordered_map<OrderId, Order> orders_;
};

}
