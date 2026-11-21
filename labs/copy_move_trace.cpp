#include <iostream>
#include <utility>

struct Trace {
    Trace() { std::cout << "construct\n"; }
    Trace(const Trace&) { std::cout << "copy construct\n"; }
    Trace(Trace&&) noexcept { std::cout << "move construct\n"; }
    ~Trace() { std::cout << "destroy\n"; }
};

int main() {
    Trace original;
    Trace copy{original};
    Trace moved{std::move(copy)};
}
