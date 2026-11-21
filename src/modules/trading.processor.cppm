module;
#include <unordered_map>

export module trading.processor;
export import trading.order;

export namespace trading {

enum class SubmitResult { Accepted, InvalidOrder, DuplicateId };

class OrderProcessor {
public:
    [[nodiscard]] SubmitResult submit(const Order& order);

private:
    std::unordered_map<OrderId, Order> orders_;
};

}
