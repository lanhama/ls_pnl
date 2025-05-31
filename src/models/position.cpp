#include "models/position.hpp"
#include <algorithm>
#include <iostream>

namespace position
{

    std::optional<double> Position::addTrade(const Trade &trade)
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

            // Process trades in FIFO order
            while (remainingQuantity > 0 && !trades_.empty())
            {
                Trade &oldestTrade = trades_.front();
                int32_t oldestQuantity = oldestTrade.getSide() == Side::BUY ? static_cast<int32_t>(oldestTrade.getQuantity()) : -static_cast<int32_t>(oldestTrade.getQuantity());

                int32_t closingQuantity = std::min(remainingQuantity, std::abs(oldestQuantity));

                // Calculate P&L for this portion
                double pnl = closingQuantity * (trade.getPrice() - oldestTrade.getPrice()) * (getCurrentPosition() > 0 ? 1 : -1);
                tradePnL = tradePnL.has_value() ? tradePnL.value() + pnl : pnl;

                // Update quantities
                remainingQuantity -= closingQuantity;

                // Update the oldest trade's quantity
                if (closingQuantity == std::abs(oldestQuantity))
                {
                    // Fully closed trade, remove it
                    trades_.pop_front();
                }
                else
                {
                    // Partially closed trade, update its quantity
                    oldestTrade = Trade(
                        oldestTrade.getSymbol(),
                        oldestTrade.getSide(),
                        oldestTrade.getQuantity() - closingQuantity,
                        oldestTrade.getPrice(),
                        oldestTrade.getTimestamp());
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
                trades_.push_back(excessTrade);
            }
        }
        else
        {
            // Opening or adding to position, always add the trade
            trades_.push_back(trade);
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

    std::optional<double> Positions::addTrade(const Trade &trade)
    {
        return positions_[trade.getSymbol().getSymbol()].addTrade(trade);
    }

    Position &Positions::getPosition(const std::string &symbol)
    {
        return positions_[symbol];
    }

    bool Positions::hasPosition(const std::string &symbol) const
    {
        return positions_.find(symbol) != positions_.end();
    }

} // namespace position