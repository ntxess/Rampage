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
	void read(rapidjson::Value& val, ConfigKey& configId, ConfigData& configMap);
	void read(rapidjson::Value& val, DataMap& dataMap, std::string key);
	void write(rapidjson::Value& val, ConfigKey& configId, const ConfigData& configMap, rapidjson::Document& doc);
	void write(rapidjson::Document& doc, const DataMap& dataMap);
	std::any anyCast(rapidjson::Value& val);
	bool createVal(rapidjson::Document& doc, rapidjson::Value& val, const std::any& data);
	void vecParseHelper(rapidjson::Value& val, DataMap& dataMap, std::string key, std::vector<std::any>& vec);
	SystemStatus creatConfig(const std::string& filename);

private:
	const std::filesystem::path RELATIVE_PATH;
	const std::string MAIN_CONFIG;
	const std::string CONFIG_FOLDER_PATH;
};