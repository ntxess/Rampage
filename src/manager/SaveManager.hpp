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
private:
	const std::string RELATIVE_PATH;
	const std::string SAVE_FOLDER;

public:
	SaveManager();
	SaveManager(const SaveManager&) = default;
	~SaveManager() = default;

	SystemStatus load(const std::string& filename, DataMap& dataMap);
	SystemStatus save(const std::string& filename, const DataMap& dataMap);

private:
	void read(rapidjson::Value& val, DataMap& dataMap, std::string key);
	void write(rapidjson::Document& doc, const DataMap& dataMap);
	std::any anyCast(rapidjson::Value& val);
	bool createVal(rapidjson::Document& doc, rapidjson::Value& val, const std::string& key, const std::any& data);
};