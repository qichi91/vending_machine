#include "Money.hpp"
#include <algorithm>
#include <vector>
#include <set>

namespace vending_machine::domain {

Money::Money(int amount) : amount_(amount) {
    if (amount < 0) {
        throw std::invalid_argument("Negative money amount is not allowed: " + std::to_string(amount));
    }
}

int Money::amount() const {
    return amount_;
}

bool Money::isValidDenomination() const {
    static const std::set<int> validAmounts = {10, 50, 100, 500, 1000};
    return validAmounts.find(amount_) != validAmounts.end();
}

bool Money::operator==(const Money& other) const {
    return amount_ == other.amount_;
}

bool Money::operator!=(const Money& other) const {
    return !(*this == other);
}

bool Money::operator<(const Money& other) const {
    return amount_ < other.amount_;
}

Money Money::operator+(const Money& other) const {
    return Money(amount_ + other.amount_);
}

} // namespace vending_machine::domain
