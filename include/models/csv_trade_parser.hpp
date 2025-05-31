#ifndef CSV_TRADE_PARSER_HPP
#define CSV_TRADE_PARSER_HPP

#include "trade.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

class CSVTradeParser
{
public:
    static std::vector<Trade> parseFile(const std::string &filename)
    {
        std::vector<Trade> trades;
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw std::runtime_error("Could not open file: " + filename);
        }

        // Skip header line
        std::string header;
        std::getline(file, header);

        std::string line;
        while (std::getline(file, line))
        {
            trades.push_back(parseLine(line));
        }

        return trades;
    }

private:
    static Trade parseLine(const std::string &line)
    {
        std::stringstream ss(line);
        std::string timestamp, symbol, side, quantity, price;

        // Parse CSV fields
        std::getline(ss, timestamp, ',');
        std::getline(ss, symbol, ',');
        std::getline(ss, side, ',');
        std::getline(ss, price, ',');
        std::getline(ss, quantity, ',');

        // Remove any whitespace
        timestamp = trim(timestamp);
        symbol = trim(symbol);
        side = trim(side);
        price = trim(price);
        quantity = trim(quantity);

        // Convert and create Trade object
        return Trade(
            TradeSymbol(symbol),
            sideFromString(side),
            std::stoi(quantity),
            std::stod(price),
            std::stoull(timestamp));
    }

    static std::string trim(const std::string &str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
};

#endif // CSV_TRADE_PARSER_HPP