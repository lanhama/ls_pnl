#include <gtest/gtest.h>
#include "../include/models/command_line_parser.hpp"
#include "../include/models/accounting_method.hpp"

TEST(CommandLineParserTest, ValidArguments)
{
    const char *argv[] = {"program", "test_trades.csv", "fifo"};
    auto args = CommandLineParser::parse(3, const_cast<char **>(argv));
    EXPECT_EQ(args.method, AccountingMethod::FIFO);
    EXPECT_EQ(args.csvPath, "test_trades.csv");
}

TEST(CommandLineParserTest, InvalidMethod)
{
    const char *argv[] = {"program", "test.csv", "invalid"};
    EXPECT_THROW(CommandLineParser::parse(3, const_cast<char **>(argv)),
                 std::invalid_argument);
}

TEST(CommandLineParserTest, WrongNumberOfArguments)
{
    const char *argv[] = {"program", "test.csv"};
    EXPECT_THROW(CommandLineParser::parse(2, const_cast<char **>(argv)),
                 std::invalid_argument);
}