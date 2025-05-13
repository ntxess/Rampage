#include "GameDataStoreSerializer.hpp"

GameDataStoreSerializer::GameDataStoreSerializer()
    : IDataStoreSerializer()
{}

GameDataStoreSerializer::GameDataStoreSerializer(std::filesystem::path path)
    : IDataStoreSerializer(path)
{}

bool GameDataStoreSerializer::load(std::string_view filename, DataStore& dataStore)
{
    return false;
}

bool GameDataStoreSerializer::save(std::string_view filename, const DataStore& dataStore)
{
    return false;
}