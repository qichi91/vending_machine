#pragma once

#include <set>
#include <stdexcept>
#include <string>

namespace vending_machine::domain {

/**
 * @brief Amount of money.
 * Represent general monetary value. Values must be non-negative.
 */
class Money {
public:
    explicit Money(int amount);

    int amount() const;

    /**
     * @brief Checks if this amount corresponds to a valid coin or bill.
     * @return true if amount is 10, 50, 100, 500, or 1000.
     */
    bool isValidDenomination() const;

    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator<(const Money& other) const;
    Money operator+(const Money& other) const;

private:
    int amount_;
};

} // namespace vending_machine::domain
