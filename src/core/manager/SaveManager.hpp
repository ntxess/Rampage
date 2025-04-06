#pragma once

#include "ConfigKey.hpp"
#include "../util/Logger.hpp"
#include "../util/AnyToString.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include <any>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>

class SaveManager
{
public:
    SaveManager();
    SaveManager(const std::string filepath);

    bool init(std::unordered_map<ConfigKey, std::any>& configMap);
    bool load(std::string_view filename, std::unordered_map<ConfigKey, std::any>& configMap);
    bool load(std::string_view filename, std::unordered_map<std::string, std::any>& dataMap);
    bool save(std::string_view filename, const std::unordered_map<ConfigKey, std::any>& configMap);
    bool save(std::string_view filename, const std::unordered_map<std::string, std::any>& dataMap);

    static std::string getType(const std::any& data);
    static std::string getValue(const std::any& data);
    static bool getValue(const std::any& data, std::string& retVal);
    static int getInt(const std::any& data);
    static bool getInt(const std::any& data, int& retVal);
    static long getLong(const std::any& data);
    static bool getLong(const std::any& data, long& retVal);
    static bool getBool(const std::any& data);
    static bool getBool(const std::any& data, bool& retVal);
    static double getDouble(const std::any& data);
    static bool getDouble(const std::any& data, double& retVal);
    static std::string getString(const std::any& data);
    static bool getString(const std::any& data, std::string& retVal);
    static std::vector<std::any> getVec(const std::any& data);
    static bool getVec(const std::any& data, std::vector<std::any>& retVal);
    std::filesystem::path resolvePath(std::string path);

private:
    void read(rapidjson::Value& val, ConfigKey& configId, std::unordered_map<ConfigKey, std::any>& configMap);
    void read(rapidjson::Value& val, std::unordered_map<std::string, std::any>& dataMap, std::string key);
    void write(rapidjson::Value& val, ConfigKey& configId, const std::unordered_map<ConfigKey, std::any>& configMap, rapidjson::Document& doc);
    void write(rapidjson::Document& doc, const std::unordered_map<std::string, std::any>& dataMap);
    std::any anyCast(rapidjson::Value& val);
    bool createVal(rapidjson::Document& doc, rapidjson::Value& val, const std::any& data);
    void vecParseHelper(rapidjson::Value& val, std::unordered_map<std::string, std::any>& dataMap, std::string key, std::vector<std::any>& vec);
    bool creatConfig(std::string_view filename);

private:
    const std::filesystem::path RELATIVE_PATH;
    const std::string MAIN_CONFIG;
    const std::string CONFIG_FOLDER_PATH;
};

inline std::ostream& operator<<(std::ostream& os, const std::unordered_map<ConfigKey, std::any>& cd)
{
    for (const auto& [key, val] : cd)
    {
        os << "\t{ " << key << ", " << SaveManager::getValue(val) << " } | Type: " << SaveManager::getType(val) << "\n";
    }

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, std::any>& dm)
{
    for (const auto& [key, val] : dm)
    {
        os << "\t{ " << key << ", " << SaveManager::getValue(val) << " } | Type: " << SaveManager::getType(val) << "\n";
    }

    return os;
}