#include <iostream>
#include <memory>
#include <utility>

int main() {
    auto source = std::make_unique<int[]>(3);
    source[0] = 42;

    auto destination = std::move(source);
    if (source || !destination || destination[0] != 42) {
        return 1;
    }
    std::cout << "First value after ownership transfer: " << destination[0] << '\n';
}
