#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include "models/accounting_method.hpp"
#include <string>
#include <filesystem>
#include <stdexcept>

class CommandLineParser
{
public:
    struct Arguments
    {
        std::string csvPath;
        AccountingMethod method;
    };

    static Arguments parse(int argc, char *argv[])
    {
        if (argc != 3)
        {
            throw std::invalid_argument(
                "Usage: " + std::string(argv[0]) + " <csv_file> <method>\n"
                                                   "  method must be either 'fifo' or 'lifo'");
        }

        Arguments args;
        args.csvPath = argv[1];
        args.method = parseMethod(argv[2]);

        if (!std::filesystem::exists(args.csvPath))
        {
            throw std::runtime_error("File '" + args.csvPath + "' does not exist");
        }

        return args;
    }

private:
    static AccountingMethod parseMethod(const std::string &method)
    {
        if (method == "fifo")
            return AccountingMethod::FIFO;
        if (method == "lifo")
            return AccountingMethod::LIFO;
        throw std::invalid_argument("Method must be either 'fifo' or 'lifo'");
    }
};

#endif // COMMAND_LINE_PARSER_HPP