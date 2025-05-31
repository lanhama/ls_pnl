#ifndef TRADE_HPP
#define TRADE_HPP

#include "models/side.hpp"
#include "models/symbol.hpp"
#include <cstdint>
#include <stdexcept>

class Trade
{
public:
    Trade(const TradeSymbol &symbol,
          const Side &side,
          uint32_t quantity,
          double price,
          uint64_t timestamp) : symbol_(symbol),
                                side_(side),
                                quantity_(quantity),
                                price_(price),
                                timestamp_(timestamp)
    {
        if (price <= 0)
        {
            throw std::invalid_argument("Price must be positive");
        }
    }

    const TradeSymbol &getSymbol() const { return symbol_; }
    const Side &getSide() const { return side_; }
    const uint32_t getQuantity() const { return quantity_; }
    double getPrice() const { return price_; }
    uint64_t getTimestamp() const { return timestamp_; }

private:
    TradeSymbol symbol_;
    Side side_;
    uint32_t quantity_;
    double price_;
    uint64_t timestamp_;
};

#endif // TRADE_HPP