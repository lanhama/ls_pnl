#pragma once

#include "models/trade.hpp"
#include "models/accounting_method.hpp"
#include <map>
#include <string>
#include <vector>
#include <deque>

namespace position
{
    // Represents a single position with its associated trades
    class Position
    {
    public:
        Position() = default;
        ~Position() = default;

        // Add a trade to this position
        // Returns the PnL generated from this trade (std::nullopt if not a closing trade)
        std::optional<double> addTrade(const Trade &trade, AccountingMethod method);

        void printTrades() const;

        int32_t getCurrentPosition() const;

    private:
        std::deque<Trade> trades_; // Using deque for efficient FIFO/LIFO operations
    };
} // namespace position