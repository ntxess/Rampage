#include "DataStore.hpp"

bool DataStore::contains(std::string_view key) const
{
    return (m_data.find(key) != m_data.end());
}

void DataStore::remove(std::string_view key)
{
    if (auto it = m_data.find(key); it != m_data.end())
    {
        m_data.erase(it);
    }
}

void DataStore::clear()
{
    m_data.clear();
}

std::unordered_map<std::string, std::any, TransparentHash, TransparentEqual>& DataStore::data()
{
    return m_data;
}

const std::unordered_map<std::string, std::any, TransparentHash, TransparentEqual>& DataStore::data() const
{
    return m_data;
}

std::unordered_map<std::string, std::any>::iterator DataStore::begin()
{
    return m_data.begin();
}

std::unordered_map<std::string, std::any>::iterator DataStore::end()
{
    return m_data.end();
}

std::unordered_map<std::string, std::any>::const_iterator DataStore::begin() const
{
    return m_data.begin();
}

std::unordered_map<std::string, std::any>::const_iterator DataStore::end() const
{
    return m_data.end();
}
