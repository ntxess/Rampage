#include "../src/core/GlobalData.hpp"
#include "../src/core/util/AnyToString.hpp"
#include <any>
#include <iostream>
#include <string>
#include <unordered_map>

static int saveData(GlobalData& m_data)
{
    m_data.saveData[entt::hashed_string("unittest-save")] = std::make_unique<std::unordered_map<std::string, std::any>>();
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT00", false);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT01", 1);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT02", 2);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT03", "four");
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT04", std::vector<std::any>{"num1", "num2", "num3"});
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT05", std::vector<std::any>{100, 200, 300});
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT06", std::vector<std::any>{511.2, 611.3, 711.4});
    m_data.saveManager.save("save/unittest-save.sav", *(m_data.saveData[entt::hashed_string("unittest-save")]));

    std::unordered_map<std::string, std::any> loadedSaveData;
    m_data.saveManager.load("save/unittest-save.sav", loadedSaveData);

    int size = 0;
    for (const auto& [key, val] : *(m_data.saveData[entt::hashed_string("unittest-save")]))
    {
        auto saved = anyToString(val);
        auto loaded = anyToString(loadedSaveData.at(key));
        std::cout << key << "[" << saved << "]";
        if (saved == loaded)
        {
            std::cout << " == ";
            ++size;
        }
        else
        {
            std::cout << " != ";
        }
        std::cout << key << "[" << loaded << "]\n";
    }

    return size == loadedSaveData.size();
}

static int saveData2(GlobalData& m_data)
{
    std::unordered_map<std::string, std::any> newSaveData = {
        {"DAT00", true},
        {"DAT01", 10},
        {"DAT02", 20},
        {"DAT03", "forty"},
        {"DAT04", std::vector<std::any>{"num10", "num20", "42340"}},
        {"DAT05", std::vector<std::any>{32120, 2000, 3000}},
        {"DAT06", std::vector<std::any>{5110.2, 6110.3, 7110.4}},
    };
    m_data.saveManager.save("save/unittest-save2.sav", newSaveData);

    std::unordered_map<std::string, std::any> loadedSaveData;
    m_data.saveManager.load("save/unittest-save2.sav", loadedSaveData);

    int size = 0;
    for (const auto& [key, val] : newSaveData)
    {
        auto saved = anyToString(val);
        auto loaded = anyToString(loadedSaveData.at(key));
        std::cout << key << "[" << saved << "]";
        if (saved == loaded)
        {
            std::cout << " == ";
            ++size;
        }
        else
        {
            std::cout << " != ";
        }
        std::cout << key << "[" << loaded << "]\n";
    }

    return size == loadedSaveData.size();
}

static int saveConfig(GlobalData& m_data)
{
    m_data.saveManager.load("config.json", m_data.configData);
    m_data.saveManager.save("unittest-config.json", m_data.configData);

    ConfigData loadConfigData;
    m_data.saveManager.load("unittest-config.json", loadConfigData);

    int size = 0;
    for (const auto& [key, val] : m_data.configData)
    {
        auto saved = anyToString(val);
        auto loaded = anyToString(loadConfigData.at(key));
        std::cout << key << "[" << saved << "]";
        if (saved == loaded)
        {
            std::cout << " == ";
            ++size;
        }
        else
        {
            std::cout << " != ";
        }
        std::cout << key << "[" << loaded << "]\n";
    }

    return size == loadConfigData.size();
}

static int modifyConfig(GlobalData& m_data)
{
    m_data.saveManager.load("config.json", m_data.configData);
    m_data.saveManager.save("unittest-config.json", m_data.configData);

    std::cout << "Before config file modification\n";
    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    m_data.configData[ConfigKey::NAME] = "unittest-config";
    m_data.saveManager.save("unittest-config.json", m_data.configData);
    m_data.saveManager.load("config.json", m_data.configData);

    ConfigData loadConfigData;
    m_data.saveManager.load("unittest-config.json", loadConfigData);

    std::cout << "\nAfter config file modification\n";
    for (const auto& [key, val] : loadConfigData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return anyToString(m_data.configData[ConfigKey::NAME]) != anyToString(loadConfigData[ConfigKey::NAME]);
}

static void viewer(GlobalData& m_data)
{
    m_data.saveManager.load("config.json", m_data.configData);

    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << " | Type: " << m_data.saveManager.getType(val) << "\n";
    }
}

static int test_saveManager()
{
    GlobalData m_data;
    std::cout << "=========================================================\n";
    std::cout << "=== [Saving data to file] ===\n";
    if (saveData(m_data))
        std::cout << "Success\n\n";
    else
        std::cout << "Failed\n\n";

    std::cout << "=== [Saving data to file 2] ===\n";
    if (saveData2(m_data))
        std::cout << "Success\n\n";
    else
        std::cout << "Failed\n\n";

    std::cout << "=== [Saving config to file] ===\n";
    if (saveConfig(m_data))
        std::cout << "Success\n\n";
    else
        std::cout << "Failed\n\n";

    std::cout << "=== [Modifying config and saving to file] ===\n";
    if (modifyConfig(m_data))
        std::cout << "Success\n\n";
    else
        std::cout << "Failed\n\n";

    std::cout << "=== [Reading config file] ===\n";
    viewer(m_data);

    return 0;
}