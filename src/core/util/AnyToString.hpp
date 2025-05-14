#pragma once

#include <any>
#include <string>
#include <vector>

inline std::string anyToString(const std::any& val)
{
    if (auto x = std::any_cast<std::string>(&val))
        return *x;
    else if (auto x = std::any_cast<const char*>(&val))
        return *x;
    else if (auto x = std::any_cast<bool>(&val))
        return (std::to_string(*x) == "1") ? "true" : "false";
    else if (auto x = std::any_cast<int>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<float>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<double>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<std::vector<std::any>>(&val))
    {
        std::string result = "[";
        for (size_t i = 0; i < x->size(); ++i)
        {
            result += anyToString(x->at(i));
            if (i + 1 < x->size())
                result += ", ";
        }
        result.push_back(']');
        return result;
    }
    return std::string{};
}