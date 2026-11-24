#include <iostream>
#include <format>

import trading.processor;

int main() {
    trading::OrderProcessor processor;
    const trading::Order order{
        .id = 101, .instrument = {1}, .side = trading::Side::Buy, .price = 12500, .quantity = 100
    };
    if (const auto response = processor.submit(order); response.Result == trading::SubmitResult::Rejected) {
        std::cerr << std::format("Sample order was rejected: {}\n", trading::reason_name(response.RejectReason));
        return 1;
    }
    std::cout << "ACCEPTED " << order.id << '\n';
}
