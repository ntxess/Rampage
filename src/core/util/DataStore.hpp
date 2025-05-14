#pragma once

#include "../util/Logger.hpp"
#include <any>
#include <ostream>
#include <optional>
#include <string>
#include <unordered_map>

struct TransparentHash 
{
    using is_transparent = void;

    size_t operator()(std::string_view str) const noexcept 
    {
        return std::hash<std::string_view>{}(str);
    }

    size_t operator()(const std::string& str) const noexcept 
    {
        return std::hash<std::string>{}(str);
    }
};

struct TransparentEqual 
{
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept 
    {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept 
    {
        return lhs == rhs;
    }

    bool operator()(const std::string& lhs, std::string_view rhs) const noexcept 
    {
        return lhs == rhs;
    }

    bool operator()(std::string_view lhs, const std::string& rhs) const noexcept 
    {
        return lhs == rhs;
    }
};

class DataStore
{
public:
    template<typename T>
    std::optional<T> get(std::string_view key) const;

    template<typename T>
    void set(std::string_view key, T&& val);

    bool contains(std::string_view key) const;
    void remove(std::string_view key);
    void clear();

    std::unordered_map<std::string, std::any, TransparentHash, TransparentEqual>& data();
    const std::unordered_map<std::string, std::any, TransparentHash, TransparentEqual>& data() const;

    std::unordered_map<std::string, std::any>::iterator begin();
    std::unordered_map<std::string, std::any>::iterator end();
    std::unordered_map<std::string, std::any>::const_iterator begin() const;
    std::unordered_map<std::string, std::any>::const_iterator end() const;

private:
    std::unordered_map<std::string, std::any, TransparentHash, TransparentEqual> m_data;
};

template<typename T>
inline std::optional<T> DataStore::get(std::string_view key) const
{
    if (auto it = m_data.find(key); it != m_data.end()) 
    {
        try 
        {
            return std::any_cast<T>(it->second);
        }
        catch (const std::bad_any_cast& e) 
        {
            LOG_ERROR(Logger::get()) << "Failed to cast value for key: " << key << ". Error: " << e.what();
            return std::nullopt;
        }
    }
    return std::nullopt;
}

template<typename T>
inline void DataStore::set(std::string_view key, T&& value)
{
    m_data[std::string(key)] = std::forward<T>(value);
}

inline std::string getType(const std::any& data)
{
    if (std::any_cast<std::string>(&data))
        return "string";
    else if (std::any_cast<const char*>(&data))
        return "cstring";
    else if (std::any_cast<bool>(&data))
        return "bool";
    else if (std::any_cast<int>(&data))
        return "int";
    else if (std::any_cast<float>(&data))
        return "float";
    else if (std::any_cast<double>(&data))
        return "double";
    else if (auto x = std::any_cast<std::vector<std::any>>(&data))
    {
        std::string result = "[";
        for (size_t i = 0; i < (*x).size(); ++i)
        {
            result += getType((*x)[i]);
            if (i + 1 < (*x).size())
                result += ", ";
        }
        result.push_back(']');
        return result;
    }
    return "Undefined Type";
}

inline std::string getValue(const std::any& data)
{
    if (data.has_value())
    {
        try
        {
            if (auto x = std::any_cast<std::string>(&data))
                return *x;
            else if (auto x = std::any_cast<const char*>(&data))
                return *x;
            else if (auto x = std::any_cast<bool>(&data))
                return (std::to_string(*x) == "1") ? "true" : "false";
            else if (auto x = std::any_cast<int>(&data))
                return std::to_string(*x);
            else if (auto x = std::any_cast<float>(&data))
                return std::to_string(*x);
            else if (auto x = std::any_cast<double>(&data))
                return std::to_string(*x);
            else if (auto x = std::any_cast<std::vector<std::any>>(&data))
            {
                std::string result = "[";
                for (size_t i = 0; i < (*x).size(); ++i)
                {
                    result += getValue((*x)[i]);
                    if (i + 1 < (*x).size())
                        result += ", ";
                }
                result.push_back(']');
                return result;
            }
        }
        catch (const std::bad_any_cast& e)
        {
            LOG_ERROR(Logger::get()) << "Failed to cast value. Error: " << e.what();
            return std::string{};
        }
    }
    return std::string{};
}

inline std::ostream& operator<<(std::ostream& os, const DataStore& ds)
{
    for (const auto& [key, val] : ds)
    {
        os << "\t{ " << key << ", " << getValue(val) << " } | Type: " << getType(val) << "\n";
    }

    return os;
}
