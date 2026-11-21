module trading.processor;

namespace trading {

SubmitResult OrderProcessor::submit(const Order& order) {
    if (!order.is_valid()) {
        return SubmitResult::InvalidOrder;
    }
    const auto insertion = orders_.try_emplace(order.id, order);
    return insertion.second ? SubmitResult::Accepted : SubmitResult::DuplicateId;
}

}
