#include <gtest/gtest.h>
#include "../include/models/csv_trade_parser.hpp"
#include <fstream>
#include <filesystem>

TEST(CSVParserTest, ParseValidCSV)
{
    auto trades = CSVTradeParser::parseFile("test_trades.csv");

    ASSERT_EQ(trades.size(), 4);

    // Test first trade
    EXPECT_EQ(trades[0].getSymbol().getSymbol(), "TFS");
    EXPECT_EQ(trades[0].getSide(), Side::BUY);
    EXPECT_EQ(trades[0].getQuantity(), 15);
    EXPECT_DOUBLE_EQ(trades[0].getPrice(), 11.00);

    // Test second trade
    EXPECT_EQ(trades[1].getSymbol().getSymbol(), "TFS");
    EXPECT_EQ(trades[1].getSide(), Side::BUY);
    EXPECT_EQ(trades[1].getQuantity(), 15);
    EXPECT_DOUBLE_EQ(trades[1].getPrice(), 12.50);

    // Test third trade
    EXPECT_EQ(trades[2].getSymbol().getSymbol(), "TFS");
    EXPECT_EQ(trades[2].getSide(), Side::SELL);
    EXPECT_EQ(trades[2].getQuantity(), 20);
    EXPECT_DOUBLE_EQ(trades[2].getPrice(), 13.00);

    // Test fourth trade
    EXPECT_EQ(trades[3].getSymbol().getSymbol(), "TFS");
    EXPECT_EQ(trades[3].getSide(), Side::SELL);
    EXPECT_EQ(trades[3].getQuantity(), 10);
    EXPECT_DOUBLE_EQ(trades[3].getPrice(), 12.75);
}