#include <iostream>
#include "models/command_line_parser.hpp"
#include "models/csv_trade_parser.hpp"
#include "models/accountant.hpp"

int main(int argc, char *argv[])
{
    try
    {
        auto args = CommandLineParser::parse(argc, argv);
        auto trades = CSVTradeParser::parseFile(args.csvPath);

        accountant::Accountant accountant(args.method);
        std::cout << "TIMESTAMP,SYMBOL,PNL" << std::endl;
        for (const auto &trade : trades)
        {
            std::optional<double> pnl = accountant.addTrade(trade, args.method);
            if (pnl.has_value())
            {
                std::cout << trade.getTimestamp() << "," << trade.getSymbol().getSymbol() << "," << std::fixed << std::setprecision(2) << pnl.value() << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}