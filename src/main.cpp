#include <iostream>

import trading.processor;

int main() {
    trading::OrderProcessor processor;
    const trading::Order order{
        .id = 101, .instrument = {1}, .side = trading::Side::Buy, .price = 12500, .quantity = 100
    };
    if (processor.submit(order) != trading::SubmitResult::Accepted) {
        std::cerr << "Sample order was rejected\n";
        return 1;
    }
    std::cout << "ACCEPTED " << order.id << '\n';
}
