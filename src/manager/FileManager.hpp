#pragma once

#include "../common/CommonEnum.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include <any>
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

class FileManager
{
private:
	const std::string RELATIVE_PATH;
	const std::string MAIN_CONFIG;

public:
	FileManager();
	FileManager(const std::string filepath);
	FileManager(const FileManager&) = default;
	~FileManager() = default;
	SystemStatus init(DataMap& datamap);
	SystemStatus load(const std::string& filepath, DataMap& datamap);
	SystemStatus save(const std::string& filepath, DataMap& datamap);

private:
	void read(rapidjson::Value& val, DataKey& nodeId, DataMap& datamap);
	void write(rapidjson::Value& val, DataKey& nodeId, const DataMap& datamap, rapidjson::Document& doc);
	std::any resolveType(rapidjson::Value& key);
	SystemStatus creatConfig(const std::string& filepath);
};
