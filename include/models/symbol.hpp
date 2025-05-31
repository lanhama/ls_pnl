#ifndef TRADING_SYMBOL_HPP
#define TRADING_SYMBOL_HPP

#include <string>
#include <stdexcept>

class TradeSymbol
{
public:
    explicit TradeSymbol(const std::string &symbol)
    {
        if (symbol.empty())
        {
            throw std::invalid_argument("Symbol cannot be empty");
        }
        symbol_ = symbol;
    }

    const std::string &getSymbol() const { return symbol_; }

    bool operator==(const TradeSymbol &other) const
    {
        return symbol_ == other.symbol_;
    }

private:
    std::string symbol_;
};

#endif // TRADING_SYMBOL_HPP