#include <iostream>
#include "models/command_line_parser.hpp"
#include "models/csv_trade_parser.hpp"

int main(int argc, char *argv[])
{
    try
    {
        auto args = CommandLineParser::parse(argc, argv);
        auto trades = CSVTradeParser::parseFile(args.csvPath);

        // TODO: Calculate PnL based on method
        std::cout << "Successfully parsed " << trades.size() << " trades\n";
        std::cout << "Using " << (args.method == Method::FIFO ? "FIFO" : "LIFO") << " method\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}