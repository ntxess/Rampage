#include "ConfigManager.hpp"

ConfigManager::ConfigManager()
    : RELATIVE_PATH(std::filesystem::current_path().string())
    , MAIN_CONFIG("/config/config.json")
{}

ConfigManager::ConfigManager(const std::string filepath)
    : RELATIVE_PATH(std::filesystem::current_path().string())
    , MAIN_CONFIG(filepath)
{}

SystemStatus ConfigManager::init(ConfigMap& configMap)
{
    std::vector<ConfigKey> paths = { SAVE_FOLDER, SCRIPT_FOLDER };

    try
    {
        std::filesystem::create_directories(RELATIVE_PATH + "/config/");

        if (configMap.empty())
        {
            creatConfig(MAIN_CONFIG);

            if (load(MAIN_CONFIG, configMap) == SystemStatus::FILE_MNGR_SUCCESS)
            {
                for (const auto& path : paths)
                {
                    if (anyToString(configMap[path]).empty()) continue;
                    std::filesystem::create_directories(RELATIVE_PATH + anyToString(configMap[path]));
                }
            }
        }
    }
    catch (...)
    {
        return creatConfig(RELATIVE_PATH + MAIN_CONFIG);
    }

    return SystemStatus::FILE_MNGR_SUCCESS;
}

SystemStatus ConfigManager::load(const std::string& filepath, ConfigMap& configMap)
{
    std::ifstream ifs(RELATIVE_PATH + filepath, std::fstream::app);

    if (!ifs.good()) 
        return SystemStatus::FILE_MNGR_FAIL_READ;

    if (std::filesystem::is_empty(RELATIVE_PATH + filepath))
        return SystemStatus::FILE_MNGR_FAIL_FILE_EMPTY;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    ConfigKey configId = NAME;
    read(doc, configId, configMap);

    return SystemStatus::FILE_MNGR_SUCCESS;;
}

SystemStatus ConfigManager::save(const std::string& filepath, ConfigMap& configMap)
{
    std::ifstream ifs(RELATIVE_PATH + filepath, std::fstream::app);

    if (!ifs.good())
        return SystemStatus::FILE_MNGR_FAIL_READ;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    ConfigKey datakey = ConfigKey(0);
    write(doc, datakey, configMap, doc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());

    std::ofstream ofs(RELATIVE_PATH + filepath);
    if (!ofs.good()) return SystemStatus::FILE_MNGR_FAIL_WRITE;

    ofs << json;

    return SystemStatus::FILE_MNGR_SUCCESS;
}

void ConfigManager::read(rapidjson::Value& val, ConfigKey& configId, ConfigMap& configMap)
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
        rapidjson::StringBuffer sb;
        configMap[configId] = resolveType(val);
        configId = ConfigKey(int(configId) + 1);
    }
}

void ConfigManager::write(rapidjson::Value& val, ConfigKey& configId, const ConfigMap& configMap, rapidjson::Document& doc)
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
        for (rapidjson::SizeType i = 0; i < val.Size(); i++)
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

std::any ConfigManager::resolveType(rapidjson::Value& val)
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
        break;
    }
    return std::string();
}

SystemStatus ConfigManager::creatConfig(const std::string& filepath)
{
    std::fstream ifs(RELATIVE_PATH + filepath, std::fstream::in | std::fstream::out | std::fstream::app);

    if (!ifs.good()) 
        return SystemStatus::FILE_MNGR_FAIL_READ;
    
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
            "        \"path-config\": \"/config/config.json\",\n"
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
        return SystemStatus::FILE_MNGR_CREATED_FILE;
    }
    return SystemStatus::FILE_MNGR_SUCCESS;
}