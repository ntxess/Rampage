#pragma once

#include "../interface/IDataStoreSerializer.hpp"
#include "../util/DataStore.hpp"
#include "../util/Logger.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include <any>
#include <fstream>
#include <filesystem>
#include <optional>
#include <string>

class JsonDataStoreSerializer : public IDataStoreSerializer
{
public:
    JsonDataStoreSerializer();
    JsonDataStoreSerializer(std::filesystem::path path);
    ~JsonDataStoreSerializer() override = default;

    bool load(std::string_view filename, DataStore& dataStore) override;
    bool save(std::string_view filename, const DataStore& dataStore) override;

private:
    void read(std::string_view key, rapidjson::Value& val, DataStore& dataStore);
    void write(rapidjson::Document& doc, const DataStore& dataStore);
    std::optional<std::any> valueToAny(const rapidjson::Value& val);
    bool createJsonValue(rapidjson::Document& doc, rapidjson::Value& val, const std::any& data);
    void vecParseHelper(std::string_view key, rapidjson::Value& val, std::vector<std::any>& vec);
};

