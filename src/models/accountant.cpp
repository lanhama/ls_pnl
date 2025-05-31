#include "models/accountant.hpp"

std::optional<double> accountant::Accountant::addTrade(const Trade &trade, AccountingMethod method)
{
    // TODO: handle case where symbol is not found
    return positions_[trade.getSymbol().getSymbol()].addTrade(trade, method);
}

position::Position &accountant::Accountant::getPosition(const std::string &symbol)
{
    // TODO: handle case where symbol is not found
    return positions_[symbol];
}