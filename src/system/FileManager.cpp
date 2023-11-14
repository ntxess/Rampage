#include "FileManager.hpp"

FileManager::FileManager(SystemData* g_data)
    : g_data(g_data)
    , RELATIVE_PATH(std::filesystem::current_path().string() + "\\config\\")
    , MAIN_CONFIG("config.json")
{
    initSystemFolders();
    load(RELATIVE_PATH + MAIN_CONFIG, g_data->configData);
}

SystemStatus FileManager::load(const std::string& filepath, DataMap& datamap)
{
    std::fstream ifs(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!ifs.good()) return SystemStatus::FILE_MNGR_FAIL_READ;
    if (ifs.peek() == std::ifstream::traits_type::eof()) 
        return SystemStatus::FILE_MNGR_FAIL_FILE_EMPTY;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    DataKey nodeId = NAME;
    read(doc, nodeId, datamap);

    return SystemStatus::FILE_MNGR_SUCCESS;;
}

SystemStatus FileManager::save(const std::string& filepath, DataMap& datamap)
{
    std::fstream ifs(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!ifs.good()) return SystemStatus::FILE_MNGR_FAIL_READ;
    if (ifs.peek() == std::ifstream::traits_type::eof())
        return SystemStatus::FILE_MNGR_FAIL_FILE_EMPTY;

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    DataKey datakey = DataKey(0);
    write(doc, datakey, datamap, doc);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());

    std::ofstream of(filepath);
    of << json;

    if (!of.good()) return SystemStatus::FILE_MNGR_FAIL_WRITE;
    if (ifs.peek() == std::ifstream::traits_type::eof()) ifs << "{}";

    return SystemStatus::FILE_MNGR_SUCCESS;
}

void FileManager::read(rapidjson::Value& val, DataKey& nodeId, DataMap& datamap)
{
    if (val.IsObject())
    {
        for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
        {
            read(it->value, nodeId, datamap);
        }
    }
    else if (val.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < val.Size(); i++)
        {
            read(val[i], nodeId, datamap);
        }
    }
    else
    {
        rapidjson::StringBuffer sb;
        datamap[nodeId] = resolveType(val);
        nodeId = DataKey(int(nodeId) + 1);
    }
}

void FileManager::write(rapidjson::Value& val, DataKey& nodeId, const DataMap& datamap, rapidjson::Document& doc)
{
    if (val.IsObject())
    {
        for (auto it = val.MemberBegin(); it != val.MemberEnd(); ++it)
        {
            write(it->value, nodeId, datamap, doc);
        }
    }
    else if (val.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < val.Size(); i++)
        {
            write(val[i], nodeId, datamap, doc);
        }
    }
    else
    {
        switch (val.GetType())
        {
        case rapidjson::kStringType:
            val.SetString(anyToString(datamap.at(nodeId)).c_str(), doc.GetAllocator());
            break;

        case rapidjson::kNumberType:
            val.SetDouble(std::any_cast<double>(datamap.at(nodeId)));
            break;

        case rapidjson::kTrueType:
            [[fallthrough]];

        case rapidjson::kFalseType:
            val.SetBool(std::any_cast<bool>(datamap.at(nodeId)));
            break;
        }
        nodeId = DataKey(int(nodeId) + 1);
    }
}

std::any FileManager::resolveType(rapidjson::Value& key)
{
    switch (key.GetType())
    {
    case rapidjson::kStringType:
        return std::string(key.GetString());

    case rapidjson::kNumberType:
        if (key.IsInt())
            return key.GetInt();
        else if (key.IsDouble());
            return key.GetDouble();
        break;
    case rapidjson::kTrueType:
        [[fallthrough]];

    case rapidjson::kFalseType:
        return key.GetBool();

    case rapidjson::kArrayType:
        break;
    }
    return std::string();
}

SystemStatus FileManager::initSystemFolders()
{
    std::vector<DataKey> paths = { PATH_SAVE, PATH_SCRIPT };

    try
    {
        std::filesystem::create_directories(RELATIVE_PATH);

        for (const auto& path : paths)
        {
            std::filesystem::create_directories(anyToString(g_data->configData[path]));
        }
    }
    catch (...)
    {
        return creatConfig(RELATIVE_PATH + MAIN_CONFIG);
    }

    return SystemStatus::FILE_MNGR_SUCCESS;
}

SystemStatus FileManager::creatConfig(const std::string& filepath)
{
    std::fstream ifs(filepath, std::fstream::in | std::fstream::out | std::fstream::app);
    if (!ifs.good()) return SystemStatus::FILE_MNGR_FAIL_READ;

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
            "        \"path-config\": \"\\\\config\\\\config.json\",\n"
            "        \"path-save\": \"\\\\save\\\\\",\n"
            "        \"path_script\": \"\\\\script\\\\\"\n"
            "    },\n"
            "    \"video-settings\": {\n"
            "        \"resolution\": {\n"
            "            \"width\": 1280.0,\n"
            "            \"height\": 720.0\n"
            "        },\n"
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