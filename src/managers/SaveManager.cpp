#include "SaveManager.hpp"

/**
 * @brief 
*/
SaveManager::SaveManager()
	: RELATIVE_PATH(std::filesystem::current_path())
    , MAIN_CONFIG("config.json")
    , CONFIG_FOLDER_PATH("/config/")
{}

/**
 * @brief [Public] Normal Constructor. Load in a different config file on creation.
 * @param [IN] filepath
*/
SaveManager::SaveManager(const std::string filepath)
    : RELATIVE_PATH(std::filesystem::current_path())
    , MAIN_CONFIG(filepath)
    , CONFIG_FOLDER_PATH("/config/")
{}

/**
 * @brief [Public] Initialize the application's configuration. Configuration is also saved inside the
 * passed in config map.Default configuraiton is also generated if file location is inaccessible / missing.
 * Folder system is setup upon reading a valid configuration file.
 * @param [OUT] configMap
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus SaveManager::init(ConfigData& configMap)
{
    std::vector<ConfigKey> paths = { SAVE_FOLDER, SCRIPT_FOLDER };

    // Try to create a new default config file if one does not exist
    // After loading the congigurations setup the folder system for the entire application
    try
    {
        auto configPath = RELATIVE_PATH;
        std::filesystem::create_directories(configPath.append(CONFIG_FOLDER_PATH));

        if (configMap.empty())
        {
            creatConfig(MAIN_CONFIG);

            if (load(MAIN_CONFIG, configMap) == SystemStatus::SAVE_MNGR_SUCCESS)
            {
                for (const auto& path : paths)
                {
                    if (anyToString(configMap[path]).empty()) continue;
                    auto newDirPath = RELATIVE_PATH;
                    std::filesystem::create_directories(newDirPath.append(anyToString(configMap[path])));
                }
            }
        }
    }
    catch (...)
    {
        return creatConfig(MAIN_CONFIG);
    }

    return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief [Public] Load in another configuration given a filename. Configuration is saved inside a ConfigData map.
 * @param [IN] filename
 * @param [OUT] configMap
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus SaveManager::load(const std::string& filename, ConfigData& configMap)
{
    auto path = RELATIVE_PATH;
    std::ifstream ifs(path.append(CONFIG_FOLDER_PATH).append(filename).string(), std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    if (std::filesystem::is_empty(path))
        return SystemStatus::SAVE_MNGR_FAIL_FILE_EMPTY;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    ConfigKey configId = NAME;
    read(doc, configId, configMap);

    return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief
 * @param filename
 * @param dataMap
 * @return
*/
SystemStatus SaveManager::load(const std::string& filename, DataMap& dataMap)
{
    auto path = RELATIVE_PATH;
    std::ifstream ifs(path.append(filename).string(), std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    if (std::filesystem::is_empty(path))
        return SystemStatus::SAVE_MNGR_FAIL_FILE_EMPTY;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    std::string key;
    read(doc, dataMap, key);

    return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief [Public] Save/Update the file with the incoming configmap. Save file is written at the given filename.
 * @param [IN] filename
 * @param [IN] configMap
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus SaveManager::save(const std::string& filename, const ConfigData& configMap)
{
    auto path = RELATIVE_PATH;
    std::ifstream ifs(path.append(CONFIG_FOLDER_PATH).append(filename).string(), std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    if (ifs.peek() == std::ifstream::traits_type::eof())
    {
        ifs.close();
        creatConfig(filename);
        ifs.open(path, std::fstream::app);
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    ConfigKey datakey = ConfigKey(0);
    write(doc, datakey, configMap, doc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());

    std::ofstream ofs(path);
    if (!ofs.good()) return SystemStatus::SAVE_MNGR_FAIL_WRITE;

    ofs << json;

    return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief
 * @return
*/
SystemStatus SaveManager::save(const std::string& filename, const DataMap& dataMap)
{
    auto path = RELATIVE_PATH;
    std::ifstream ifs(path.append(filename).string(), std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    rapidjson::Document doc;
    doc.SetObject();
    write(doc, dataMap);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());

    std::ofstream ofs(path);
    if (!ofs.good()) return SystemStatus::SAVE_MNGR_FAIL_WRITE;

    ofs << json;

    return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief [Private] Healper function for load(). Recusively reads json file.
 * @param [IN] val
 * @param [IN] configId
 * @param [OUT] configMap
*/
void SaveManager::read(rapidjson::Value& val, ConfigKey& configId, ConfigData& configMap)
{
    if (val.IsObject())
    {
        for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
        {
            read(it->value, configId, configMap);
        }
    }
    else if (val.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < val.Size(); i++)
        {
            read(val[i], configId, configMap);
        }
    }
    else
    {
        configMap[configId] = anyCast(val);
        configId = ConfigKey(int(configId) + 1);
    }
}

/**
 * @brief
 * @param val
 * @param dataMap
 * @param key
*/
void SaveManager::read(rapidjson::Value& val, DataMap& dataMap, std::string key)
{
    if (val.IsObject())
    {
        for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
        {
            read(it->value, dataMap, it->name.GetString());
        }
    }
    else
    {
        if (val.IsArray())
        {
            std::vector<std::any> vec;
            for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
            {
                if (val[i].IsArray())
                {
                    vecParseHelper(val[i], dataMap, key, vec);
                }
                else
                {
                    vec.push_back(anyCast(val[i]));
                }
            }
            dataMap[key] = vec;
        }
        else
        {
            dataMap[key] = anyCast(val);
        }
    }
}

/**
 * @brief [Private] Healper function for save(). Recusively updates json file with configmap when configId is found.
 * @param [In] val
 * @param [In] configId
 * @param [In] configMap
 * @param [In] doc
*/
void SaveManager::write(rapidjson::Value& val, ConfigKey& configId, const ConfigData& configMap, rapidjson::Document& doc)
{
    if (val.IsObject())
    {
        for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
        {
            write(it->value, configId, configMap, doc);
        }
    }
    else if (val.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
        {
            write(val[i], configId, configMap, doc);
        }
    }
    else
    {
        switch (val.GetType())
        {
        case rapidjson::kNullType:
            break;

        case rapidjson::kStringType:
            if (!configMap.count(configId)) break;
            val.SetString(anyToString(configMap.at(configId)).c_str(), doc.GetAllocator());
            break;

        case rapidjson::kNumberType:
            if (val.IsInt())
                val.SetInt(std::any_cast<int>(configMap.at(configId)));
            else if (val.IsDouble())
                val.SetDouble(std::any_cast<double>(configMap.at(configId)));
            break;

        case rapidjson::kTrueType:
            [[fallthrough]];

        case rapidjson::kFalseType:
            val.SetBool(std::any_cast<bool>(configMap.at(configId)));
            break;

        default:
            break;
        }
        configId = ConfigKey(int(configId) + 1);
    }
}

/**
 * @brief
 * @param doc
 * @param dataMap
 * @return
*/
void SaveManager::write(rapidjson::Document& doc, const DataMap& dataMap)
{
    for (const auto& [key, data] : dataMap)
    {
        rapidjson::Value val;
        if (createVal(doc, val, key, data))
        {
            rapidjson::Value index(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), doc.GetAllocator());
            doc.AddMember(index, val, doc.GetAllocator());        
        }
    }
}

/**
 * @brief 
 * @param val 
 * @return 
*/
std::any SaveManager::anyCast(rapidjson::Value& val)
{
    switch (val.GetType())
    {
    case rapidjson::kStringType:
        return std::string(val.GetString());

    case rapidjson::kNumberType:
        if (val.IsInt())
            return val.GetInt();
        else if (val.IsDouble())
            return val.GetDouble();
        else return std::string();

    case rapidjson::kTrueType:
        [[fallthrough]];

    case rapidjson::kFalseType:
        return val.GetBool();

    case rapidjson::kArrayType:
        return val.GetArray();
    }
    return std::string();
}

/**
 * @brief
 * @param doc
 * @param val
 * @param key
 * @param data
 * @return
*/
bool SaveManager::createVal(rapidjson::Document& doc, rapidjson::Value& val, const std::string& key, const std::any& data)
{
    if (data.type() == typeid(const char*))
    {
        val.SetString(std::any_cast<const char*>(data), doc.GetAllocator());
        return true;
    }
    else if (data.type() == typeid(std::string))
    {
        val.SetString(std::any_cast<std::string>(data).c_str(), doc.GetAllocator());
        return true;
    }
    else if (data.type() == typeid(bool))
    {
        val.SetBool(std::any_cast<bool>(data));
        return true;
    }
    else if (data.type()== typeid(int))
    {
        val.SetInt(std::any_cast<int>(data));
        return true;
    }
    else if (data.type() == typeid(float))
    {
        val.SetFloat(std::any_cast<float>(data));
        return true;
    }
    else if (data.type() == typeid(double))
    {
        val.SetDouble(std::any_cast<double>(data));
        return true;
    }
    else if (data.type() == typeid(std::vector<std::any>))
    {
        const std::vector<std::any>& vecRef = std::any_cast<std::vector<std::any>>(data);
        val.SetArray();

        for (const auto& dataIndex : vecRef)
        {
            if (dataIndex.type() == typeid(const char*))
            {
                rapidjson::Value strVal;
                const char* str = std::any_cast<const char*>(dataIndex);
                strVal.SetString(str, static_cast<rapidjson::SizeType>(strlen(str)), doc.GetAllocator());
                val.PushBack(strVal, doc.GetAllocator());
            }
            else if (dataIndex.type() == typeid(std::string))
            {
                rapidjson::Value strVal;
                std::string str = std::any_cast<std::string>(dataIndex);
                strVal.SetString(str.c_str(), static_cast<rapidjson::SizeType>(str.length()), doc.GetAllocator());
                val.PushBack(strVal, doc.GetAllocator());
            }
            else if (dataIndex.type() == typeid(bool))
            {
                val.PushBack(std::any_cast<bool>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type() == typeid(int))
            {
                val.PushBack(std::any_cast<int>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type() == typeid(float))
            {
                val.PushBack(std::any_cast<float>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type() == typeid(double))
            {
                val.PushBack(std::any_cast<double>(dataIndex), doc.GetAllocator());
            }
        }
        return true;
    }
    return false;
}

void SaveManager::vecParseHelper(rapidjson::Value& val, DataMap& dataMap, std::string key, std::vector<std::any>& vec)
{
    std::vector<std::any> newVec;
    for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
    {
        if (val[i].IsArray())
        {
            vecParseHelper(val[i], dataMap, key, vec);
        }
        else
        {
            newVec.push_back(anyCast(val[i]));
        }
    }
    vec.push_back(newVec);
}

/**
 * @brief [Private] Generates a default config file with default settings.
 * @param [IN] filename
 * @return SystemStatus error code if failure; otherwise, SystemStatus::Success.
*/
SystemStatus SaveManager::creatConfig(const std::string& filename)
{
    auto path = RELATIVE_PATH;
    std::fstream ifs(path.append(CONFIG_FOLDER_PATH).append(filename).string(), std::fstream::in | std::fstream::out | std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string json = buffer.str();

    if (json.empty())
    {
        const std::string data =
            "{\n"
            "    \"system\": {\n"
            "        \"name\": \"Rampage\",\n"
            "        \"version\": \"0.1b\"\n"
            "    },\n"
            "    \"system-paths\": {\n"
            "        \"config-folder\": \"/config/\",\n"
            "        \"save-folder\": \"/save/\",\n"
            "        \"script-folder\": \"/script/\"\n"
            "    },\n"
            "    \"video-settings\": {\n"
            "        \"resolution\": {\n"
            "            \"width\": 1280,\n"
            "            \"height\": 720\n"
            "        },\n"
            "        \"frame-rate\": 165.0,\n"
            "        \"fullscreen\": false,\n"
            "        \"damage-number\": true,\n"
            "        \"visual-effects\": true,\n"
            "        \"screen-shake\": true,\n"
            "        \"mature\": true\n"
            "    },\n"
            "    \"sound-settings\": {\n"
            "        \"music-volume\": 100.0,\n"
            "        \"effects-volume\": 100.0\n"
            "    },\n"
            "    \"control-settings\": {\n"
            "        \"key-bindings\": {\n"
            "            \"key-up\": \"w\",\n"
            "            \"key-left\": \"a\",\n"
            "            \"key-down\": \"s\",\n"
            "            \"key-right\": \"d\",\n"
            "            \"key-jump\": \"space\",\n"
            "            \"key-exit\": \"esc\",\n"
            "            \"key-enter\": \"enter\",\n"
            "            \"key-back\": \"enter\",\n"
            "            \"key-1\": \"u\",\n"
            "            \"key-2\": \"i\",\n"
            "            \"key-3\": \"o\",\n"
            "            \"key-4\": \"j\",\n"
            "            \"key-5\": \"k\",\n"
            "            \"key-6\": \"l\"\n"
            "        }\n"
            "    },\n"
            "    \"language-settings\": {\n"
            "        \"dub\": \"english\",\n"
            "        \"sub\": \"english\",\n"
            "        \"subtitle\": false\n"
            "    },\n"
            "    \"accessibility-settings\": {\n"
            "        \"colorblind-mode\": false\n"
            "    }\n"
            "}";
        ifs << data;
        return SystemStatus::SAVE_MNGR_CREATED_FILE;
    }
    return SystemStatus::SAVE_MNGR_SUCCESS;
}