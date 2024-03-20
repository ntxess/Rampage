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

class SaveManager
{
public:
	SaveManager();
	SaveManager(const std::string filepath);

	SystemStatus init(ConfigData& configMap);
	SystemStatus load(const std::string& filename, ConfigData& configMap);
	SystemStatus load(const std::string& filename, DataMap& dataMap);
	SystemStatus save(const std::string& filename, const ConfigData& configMap);
	SystemStatus save(const std::string& filename, const DataMap& dataMap);

private:
	void read(rapidjson::Value& val, ConfigKey& configId, ConfigData& configMap);
	void read(rapidjson::Value& val, DataMap& dataMap, std::string key);
	void write(rapidjson::Value& val, ConfigKey& configId, const ConfigData& configMap, rapidjson::Document& doc);
	void write(rapidjson::Document& doc, const DataMap& dataMap);
	std::any anyCast(rapidjson::Value& val);
	bool createVal(rapidjson::Document& doc, rapidjson::Value& val, const std::string& key, const std::any& data);
	SystemStatus creatConfig(const std::string& filename);

private:
	const std::string RELATIVE_PATH;
	const std::string MAIN_CONFIG;
	const std::string CONFIG_FOLDER_PATH;
};