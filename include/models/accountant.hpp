#pragma once

#include "models/accounting_method.hpp"
#include "models/trade.hpp"
#include "models/position.hpp"
#include <map>
#include <string>

namespace accountant
{
    // Container to hold positions and accounting method
    class Accountant
    {
    public:
        Accountant(AccountingMethod method) : method_(method) {}
        ~Accountant() = default;

        // Add a trade to the appropriate position
        // Returns the PnL generated from this trade (std::nullopt if not a closing trade)
        std::optional<double> addTrade(const Trade &trade, AccountingMethod method);

        position::Position &getPosition(const std::string &symbol);

    private:
        std::map<std::string, position::Position> positions_;
        AccountingMethod method_;
    };
} // namespace accountant