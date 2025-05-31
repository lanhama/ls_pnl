#ifndef SIDE_HPP
#define SIDE_HPP

#include <string>
#include <stdexcept>

enum class Side
{
    BUY,
    SELL
};

// Helper functions for Side
inline Side sideFromString(const std::string &side)
{
    if (side == "B")
    {
        return Side::BUY;
    }
    else if (side == "S")
    {
        return Side::SELL;
    }
    throw std::invalid_argument("Invalid side: " + side);
}

inline std::string sideToString(Side side)
{
    if (side == Side::BUY)
    {
        return "B";
    }
    else if (side == Side::SELL)
    {
        return "S";
    }
    return "";
}

#endif // SIDE_HPP