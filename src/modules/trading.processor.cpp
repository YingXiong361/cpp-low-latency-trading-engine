module trading.processor;

namespace trading {

SubmitResponse OrderProcessor::submit(const Order& order) {
    if (!order.is_valid()) {
        return {SubmitResult::Rejected, SubmitRejectReason::InvalidOrder};
    }
    const auto insertion = orders_.try_emplace(order.id, order);
    if (!insertion.second) {
        return {SubmitResult::Rejected, SubmitRejectReason::DuplicateId};
    }
    return {.Result = SubmitResult::Accepted};
}

}
