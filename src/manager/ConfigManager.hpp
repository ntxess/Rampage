#pragma once

#include "../common/CommonEnum.hpp"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <any>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

class ConfigManager
{
private:
	const std::string RELATIVE_PATH;
	const std::string MAIN_CONFIG;

public:
	ConfigManager();
	ConfigManager(const std::string filepath);
	ConfigManager(const ConfigManager&) = default;
	~ConfigManager() = default;

	SystemStatus init(ConfigData& configMap);
	SystemStatus load(const std::string& filepath, ConfigData& configMap);
	SystemStatus save(const std::string& filepath, const ConfigData& configMap);

private:
	void read(rapidjson::Value& val, ConfigKey& configId, ConfigData& configMap);
	void write(rapidjson::Value& val, ConfigKey& configId, const ConfigData& configMap, rapidjson::Document& doc);
	std::any resolveType(rapidjson::Value& val);
	SystemStatus creatConfig(const std::string& filepath);
};