#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include <string>
#include <filesystem>
#include <stdexcept>

enum class Method
{
    FIFO,
    LIFO
};

class CommandLineParser
{
public:
    struct Arguments
    {
        std::string csvPath;
        Method method;
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
    static Method parseMethod(const std::string &method)
    {
        if (method == "fifo")
            return Method::FIFO;
        if (method == "lifo")
            return Method::LIFO;
        throw std::invalid_argument("Method must be either 'fifo' or 'lifo'");
    }
};

#endif // COMMAND_LINE_PARSER_HPP