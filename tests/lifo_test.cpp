#include <gtest/gtest.h>
#include "../include/models/accountant.hpp"
#include "../include/models/trade.hpp"
#include "../include/models/symbol.hpp"
#include "../include/models/side.hpp"
#include "../include/models/accounting_method.hpp"

using namespace accountant;
using namespace position;

TEST(LifoTest, LIFOPnLCalculation)
{
    // Create a position
    Position position;
    TradeSymbol symbol("AAPL");

    // Add three buy trades
    Trade buy1(symbol, Side::BUY, 100, 150.0, 1); // Buy 100 @ $150
    Trade buy2(symbol, Side::BUY, 50, 160.0, 2);  // Buy 50 @ $160
    Trade buy3(symbol, Side::BUY, 75, 155.0, 3);  // Buy 75 @ $155

    std::optional<double> pnl1 = position.addTrade(buy1, AccountingMethod::LIFO);
    std::optional<double> pnl2 = position.addTrade(buy2, AccountingMethod::LIFO);
    std::optional<double> pnl3 = position.addTrade(buy3, AccountingMethod::LIFO);

    // Verify position size
    EXPECT_EQ(position.getCurrentPosition(), 225); // 100 + 50 + 75

    // Add a sell trade that partially closes the position
    Trade sell1(symbol, Side::SELL, 120, 165.0, 4); // Sell 120 @ $165
    std::optional<double> pnl4 = position.addTrade(sell1, AccountingMethod::LIFO);

    // Verify position size after partial close
    EXPECT_EQ(position.getCurrentPosition(), 105); // 225 - 120

    // Calculate expected PnL (LIFO):
    // First 75 shares: (165 - 155) * 75 = 750
    // Next 45 shares: (165 - 160) * 45 = 225
    // Total PnL = 975
    EXPECT_DOUBLE_EQ(pnl4.value(), 975.0);

    // Add another sell trade to close the remaining position
    Trade sell2(symbol, Side::SELL, 105, 170.0, 5); // Sell 105 @ $170
    std::optional<double> pnl5 = position.addTrade(sell2, AccountingMethod::LIFO);

    // Verify position is closed
    EXPECT_EQ(position.getCurrentPosition(), 0);

    // Calculate expected PnL (LIFO):
    // Remaining 5 shares from second buy: (170 - 160) * 5 = 50
    // All 100 shares from first buy: (170 - 150) * 100 = 2000
    // Total PnL = 2050
    EXPECT_DOUBLE_EQ(pnl5.value(), 2050.0);
}

TEST(LifoTest, LIFOMultipleSymbols)
{
    Accountant accountant(AccountingMethod::LIFO);
    TradeSymbol aapl("AAPL");
    TradeSymbol msft("MSFT");

    // Add trades for AAPL
    Trade aaplBuy1(aapl, Side::BUY, 100, 150.0, 1);
    Trade aaplSell1(aapl, Side::SELL, 50, 160.0, 2);
    std::optional<double> pnl1 = accountant.addTrade(aaplBuy1, AccountingMethod::LIFO);
    std::optional<double> pnl2 = accountant.addTrade(aaplSell1, AccountingMethod::LIFO);

    // Add trades for MSFT
    Trade msftBuy1(msft, Side::BUY, 200, 300.0, 3);
    Trade msftSell1(msft, Side::SELL, 100, 310.0, 4);
    std::optional<double> pnl3 = accountant.addTrade(msftBuy1, AccountingMethod::LIFO);
    std::optional<double> pnl4 = accountant.addTrade(msftSell1, AccountingMethod::LIFO);

    // Verify AAPL position
    EXPECT_EQ(accountant.getPosition("AAPL").getCurrentPosition(), 50);
    EXPECT_EQ(pnl1, std::nullopt);         // Not a closing trade
    EXPECT_DOUBLE_EQ(pnl2.value(), 500.0); // (160 - 150) * 50

    // Verify MSFT position
    EXPECT_EQ(accountant.getPosition("MSFT").getCurrentPosition(), 100);
    EXPECT_EQ(pnl3, std::nullopt);          // Not a closing trade
    EXPECT_DOUBLE_EQ(pnl4.value(), 1000.0); // (310 - 300) * 100
}

TEST(LifoTest, LIFOPositionFlip)
{
    Position position;
    TradeSymbol symbol("AAPL");

    // Start with a long position
    Trade buy1(symbol, Side::BUY, 50, 150.0, 1); // Buy 50 @ $150
    std::optional<double> pnl1 = position.addTrade(buy1, AccountingMethod::LIFO);
    EXPECT_EQ(pnl1, std::nullopt);
    EXPECT_EQ(position.getCurrentPosition(), 50);

    // Sell more than we have, flipping to short
    Trade sell1(symbol, Side::SELL, 100, 160.0, 2); // Sell 100 @ $160
    std::optional<double> pnl2 = position.addTrade(sell1, AccountingMethod::LIFO);

    // Verify position is now short
    EXPECT_EQ(position.getCurrentPosition(), -50);

    // Calculate expected PnL (LIFO):
    // Close long 50: (160 - 150) * 50 = 500
    EXPECT_DOUBLE_EQ(pnl2.value(), 500.0);

    // Buy to close the short
    Trade buy2(symbol, Side::BUY, 50, 155.0, 3); // Buy 50 @ $155
    std::optional<double> pnl3 = position.addTrade(buy2, AccountingMethod::LIFO);

    // Verify position is closed
    EXPECT_EQ(position.getCurrentPosition(), 0);

    // Calculate expected PnL (LIFO):
    // Close short 50: (160 - 155) * 50 = 250
    EXPECT_DOUBLE_EQ(pnl3.value(), 250.0);
}

TEST(LifoTest, LIFOShortPosition)
{
    Position position;
    TradeSymbol symbol("AAPL");

    // Start with a short position
    Trade sell1(symbol, Side::SELL, 100, 160.0, 1); // Sell 100 @ $160
    std::optional<double> pnl1 = position.addTrade(sell1, AccountingMethod::LIFO);
    EXPECT_EQ(pnl1, std::nullopt);
    EXPECT_EQ(position.getCurrentPosition(), -100);

    // Partially close the short position
    Trade buy1(symbol, Side::BUY, 60, 155.0, 2); // Buy 60 @ $155
    std::optional<double> pnl2 = position.addTrade(buy1, AccountingMethod::LIFO);

    // Verify remaining short position
    EXPECT_EQ(position.getCurrentPosition(), -40);

    // Calculate expected PnL (LIFO):
    // Close short 60: (160 - 155) * 60 = 300
    EXPECT_DOUBLE_EQ(pnl2.value(), 300.0);

    // Close remaining short position at a loss
    Trade buy2(symbol, Side::BUY, 40, 165.0, 3); // Buy 40 @ $165
    std::optional<double> pnl3 = position.addTrade(buy2, AccountingMethod::LIFO);

    // Verify position is closed
    EXPECT_EQ(position.getCurrentPosition(), 0);

    // Calculate expected PnL (LIFO):
    // Close short 40: (160 - 165) * 40 = -200
    EXPECT_DOUBLE_EQ(pnl3.value(), -200.0);
}