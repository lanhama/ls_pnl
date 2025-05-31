#include <gtest/gtest.h>
#include "../include/models/accountant.hpp"
#include "../include/models/trade.hpp"
#include "../include/models/symbol.hpp"
#include "../include/models/accounting_method.hpp"
#include "../include/models/side.hpp"

using namespace accountant;
using namespace position;

TEST(FifoTest, FIFOPnLCalculation)
{
    // Create a position
    Position position;
    TradeSymbol symbol("AAPL");

    // Add three buy trades
    Trade buy1(symbol, Side::BUY, 100, 150.0, 1); // Buy 100 @ $150
    Trade buy2(symbol, Side::BUY, 50, 160.0, 2);  // Buy 50 @ $160
    Trade buy3(symbol, Side::BUY, 75, 155.0, 3);  // Buy 75 @ $155

    std::optional<double> pnl1 = position.addTrade(buy1, AccountingMethod::FIFO);
    std::optional<double> pnl2 = position.addTrade(buy2, AccountingMethod::FIFO);
    std::optional<double> pnl3 = position.addTrade(buy3, AccountingMethod::FIFO);

    // Verify position size
    EXPECT_EQ(position.getCurrentPosition(), 225); // 100 + 50 + 75

    // Add a sell trade that partially closes the position
    Trade sell1(symbol, Side::SELL, 120, 165.0, 4); // Sell 120 @ $165
    std::optional<double> pnl4 = position.addTrade(sell1, AccountingMethod::FIFO);

    // Verify position size after partial close
    EXPECT_EQ(position.getCurrentPosition(), 105); // 225 - 120

    // Calculate expected PnL:
    // First 100 shares: (165 - 150) * 100 = 1500
    // Next 20 shares: (165 - 160) * 20 = 100
    // Total PnL = 1600
    EXPECT_DOUBLE_EQ(pnl4.value(), 1600.0);

    // Add another sell trade to close the remaining position
    Trade sell2(symbol, Side::SELL, 105, 170.0, 5); // Sell 105 @ $170
    std::optional<double> pnl5 = position.addTrade(sell2, AccountingMethod::FIFO);

    // Verify position is closed
    EXPECT_EQ(position.getCurrentPosition(), 0);

    // Calculate expected PnL:
    // Remaining 30 shares: (170 - 160) * 30 = 300
    // Remaining 75 shares: (170 - 155) * 75 = 1125
    // Total PnL = 1425
    EXPECT_DOUBLE_EQ(pnl5.value(), 1425.0);
}

TEST(FifoTest, MultipleSymbols)
{
    Accountant accountant(AccountingMethod::FIFO);
    TradeSymbol aapl("AAPL");
    TradeSymbol msft("MSFT");

    // Add trades for AAPL
    Trade aaplBuy1(aapl, Side::BUY, 100, 150.0, 1);
    Trade aaplSell1(aapl, Side::SELL, 50, 160.0, 2);
    std::optional<double> pnl1 = accountant.addTrade(aaplBuy1, AccountingMethod::FIFO);
    std::optional<double> pnl2 = accountant.addTrade(aaplSell1, AccountingMethod::FIFO);

    // Add trades for MSFT
    Trade msftBuy1(msft, Side::BUY, 200, 300.0, 3);
    Trade msftSell1(msft, Side::SELL, 100, 310.0, 4);
    std::optional<double> pnl3 = accountant.addTrade(msftBuy1, AccountingMethod::FIFO);
    std::optional<double> pnl4 = accountant.addTrade(msftSell1, AccountingMethod::FIFO);

    // Verify AAPL position
    EXPECT_EQ(accountant.getPosition("AAPL").getCurrentPosition(), 50);
    EXPECT_EQ(pnl1, std::nullopt);         // Not a closing trade
    EXPECT_DOUBLE_EQ(pnl2.value(), 500.0); // (160 - 150) * 50

    // Verify MSFT position
    EXPECT_EQ(accountant.getPosition("MSFT").getCurrentPosition(), 100);
    EXPECT_EQ(pnl3, std::nullopt);          // Not a closing trade
    EXPECT_DOUBLE_EQ(pnl4.value(), 1000.0); // (310 - 300) * 100
}