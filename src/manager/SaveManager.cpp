#include "SaveManager.hpp"

/**
 * @brief 
*/
SaveManager::SaveManager()
	: RELATIVE_PATH(std::filesystem::current_path().string())
	, SAVE_FOLDER("/save/")
{}

/**
 * @brief 
 * @param filename 
 * @param dataMap 
 * @return 
*/
SystemStatus SaveManager::load(const std::string& filename, DataMap& dataMap)
{
	std::ifstream ifs(RELATIVE_PATH + SAVE_FOLDER + filename, std::fstream::app);

	if (!ifs.good())
		return SystemStatus::SAVE_MNGR_FAIL_READ;

	if (std::filesystem::is_empty(RELATIVE_PATH + SAVE_FOLDER + filename))
		return SystemStatus::SAVE_MNGR_FAIL_FILE_EMPTY;

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);

    std::string key = "Key";
	read(doc, dataMap, key);

	return SystemStatus::SAVE_MNGR_SUCCESS;
}

/**
 * @brief 
 * @return 
*/
SystemStatus SaveManager::save(const std::string& filename, const DataMap& dataMap)
{
    std::ifstream ifs(RELATIVE_PATH + SAVE_FOLDER + filename, std::fstream::app);

    if (!ifs.good())
        return SystemStatus::SAVE_MNGR_FAIL_READ;

    rapidjson::Document doc;
    doc.SetObject();
    write(doc, dataMap);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());

    std::ofstream ofs(RELATIVE_PATH + SAVE_FOLDER + filename);
    if (!ofs.good()) return SystemStatus::CFG_MNGR_FAIL_WRITE;

    ofs << json;

    return SystemStatus::SAVE_MNGR_SUCCESS;
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
            
            read(it->value, dataMap, val.GetString());
        }
    }
    else
    {
        if (val.IsArray())
        {
            std::vector<std::any> vec;
            for (rapidjson::SizeType i = 0; i < val.Size(); ++i)
            {
                vec.push_back(anyCast(val[i]));
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
    if (data.type().name() == typeid(const char*).name())
    {
        val.SetString(std::any_cast<const char*>(data), doc.GetAllocator());
        return true;
    }
    else if (data.type().name() == typeid(std::string).name())
    {
        val.SetString(std::any_cast<std::string>(data).c_str(), doc.GetAllocator());
        return true;
    }
    else if (data.type().name() == typeid(bool).name())
    {
        val.SetBool(std::any_cast<bool>(data));
        return true;
    }
    else if (data.type().name() == typeid(int).name())
    {
        val.SetInt(std::any_cast<int>(data));
        return true;
    }
    else if (data.type().name() == typeid(float).name())
    {
        val.SetFloat(std::any_cast<float>(data));
        return true;
    }
    else if (data.type().name() == typeid(double).name())
    {
        val.SetDouble(std::any_cast<double>(data));
        return true;
    }
    else if (data.type().name() == typeid(std::vector<std::any>).name())
    {
        const std::vector<std::any>& vecRef = std::any_cast<std::vector<std::any>>(data);
        val.SetArray();

        for (const auto& dataIndex : vecRef)
        {
            if (dataIndex.type().name() == typeid(const char*).name())
            {
                rapidjson::Value strVal;
                const char* str = std::any_cast<const char*>(dataIndex);
                strVal.SetString(str, static_cast<rapidjson::SizeType>(strlen(str)), doc.GetAllocator());
                val.PushBack(strVal, doc.GetAllocator());
            }
            else if (dataIndex.type().name() == typeid(std::string).name())
            {
                rapidjson::Value strVal;
                std::string str = std::any_cast<std::string>(dataIndex);
                strVal.SetString(str.c_str(), static_cast<rapidjson::SizeType>(str.length()), doc.GetAllocator());
                val.PushBack(strVal, doc.GetAllocator());
            }
            else if (dataIndex.type().name() == typeid(bool).name())
            {
                val.PushBack(std::any_cast<bool>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type().name() == typeid(int).name())
            {
                val.PushBack(std::any_cast<int>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type().name() == typeid(float).name())
            {
                val.PushBack(std::any_cast<float>(dataIndex), doc.GetAllocator());
            }
            else if (dataIndex.type().name() == typeid(double).name())
            {
                val.PushBack(std::any_cast<double>(dataIndex), doc.GetAllocator());
            }
        }
        return true;
    }
    return false;
}