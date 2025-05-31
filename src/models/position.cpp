#include "models/position.hpp"
#include <algorithm>
#include <iostream>

namespace position
{

    std::optional<double> Position::addTrade(const Trade &trade, AccountingMethod method)
    {
        std::optional<double> tradePnL = std::nullopt;

        // Convert trade to position quantity (negative for sells)
        int32_t positionQuantity = trade.getSide() == Side::BUY ? static_cast<int32_t>(trade.getQuantity()) : -static_cast<int32_t>(trade.getQuantity());

        // If this is a closing trade (opposite sign of current position)
        if (getCurrentPosition() != 0 && (getCurrentPosition() > 0) != (positionQuantity > 0))
        {
            int32_t remainingQuantity = std::abs(positionQuantity);
            int32_t originalQuantity = remainingQuantity;
            int32_t currentPosition = getCurrentPosition();

            while (remainingQuantity > 0 && !trades_.empty())
            {
                Trade &front_trade = trades_.front();
                int32_t frontQuantity = front_trade.getSide() == Side::BUY ? static_cast<int32_t>(front_trade.getQuantity()) : -static_cast<int32_t>(front_trade.getQuantity());

                int32_t closingQuantity = std::min(remainingQuantity, std::abs(frontQuantity));

                // Calculate P&L for this portion
                // Always use: (sell price - buy price) * quantity
                // For longs: trade is SELL, front_trade is BUY
                // For shorts: trade is BUY, front_trade is SELL
                double sellPrice = trade.getSide() == Side::SELL ? trade.getPrice() : front_trade.getPrice();
                double buyPrice = trade.getSide() == Side::BUY ? trade.getPrice() : front_trade.getPrice();
                double pnl = closingQuantity * (sellPrice - buyPrice);
                tradePnL = tradePnL.has_value() ? tradePnL.value() + pnl : pnl;

                // Update quantities
                remainingQuantity -= closingQuantity;

                // Update the front trade's quantity
                if (closingQuantity == std::abs(frontQuantity))
                {
                    // Fully closed trade, remove it
                    trades_.pop_front();
                }
                else
                {
                    // Partially closed trade, update its quantity
                    front_trade = Trade(
                        front_trade.getSymbol(),
                        front_trade.getSide(),
                        front_trade.getQuantity() - closingQuantity,
                        front_trade.getPrice(),
                        front_trade.getTimestamp());
                }
            }

            // If the trade exceeds the current position, record the excess as a new position
            if (std::abs(positionQuantity) > std::abs(currentPosition))
            {
                // Create a new trade for the excess quantity
                Trade excessTrade(
                    trade.getSymbol(),
                    trade.getSide(),
                    std::abs(positionQuantity) - std::abs(currentPosition),
                    trade.getPrice(),
                    trade.getTimestamp());

                // We need to add the excess trade to the front or back of the deque depending on the accounting method
                if (method == AccountingMethod::FIFO)
                    trades_.push_back(excessTrade);
                else
                    trades_.push_front(excessTrade);
            }
        }
        else
        {
            // Opening or adding to position, always add the trade
            if (method == AccountingMethod::FIFO)
                trades_.push_back(trade);
            else
                trades_.push_front(trade);
        }

        return tradePnL;
    }

    int32_t Position::getCurrentPosition() const
    {
        int32_t currentPosition = 0;
        for (const auto &trade : trades_)
        {
            currentPosition += trade.getSide() == Side::BUY ? static_cast<int32_t>(trade.getQuantity()) : -static_cast<int32_t>(trade.getQuantity());
        }
        return currentPosition;
    }

    void Position::printTrades() const
    {
        for (const auto &trade : trades_)
        {
            std::cout << trade.getSymbol().getSymbol() << " "
                      << sideToString(trade.getSide()) << " "
                      << trade.getQuantity() << " "
                      << trade.getPrice() << std::endl;
        }
    }
} // namespace position