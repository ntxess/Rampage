#include "../src/common/CommonEnum.hpp"
#include "../src/common/GlobalData.hpp"
#include <iostream>
#include <string>

static int saveData(GlobalData& m_data)
{
    m_data.saveData[entt::hashed_string("unittest-save")] = std::make_unique<DataMap>();
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT00", false);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT01", 1);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT02", 2);
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT03", "four");
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT04", std::vector<std::any>{"num1", "num2", "num3"});
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT05", std::vector<std::any>{100, 200, 300});
    m_data.saveData[entt::hashed_string("unittest-save")]->emplace("DAT06", std::vector<std::any>{511.2, 611.3, 711.4});
    m_data.saveManager.save("unittest-save.sav", *(m_data.saveData[entt::hashed_string("unittest-save")]));

    for (const auto& [key, val] : *(m_data.saveData[entt::hashed_string("unittest-save")]))
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int saveData2(GlobalData& m_data)
{
    DataMap newSaveData = {
        {"DAT00", true},
        {"DAT01", 10},
        {"DAT02", 20},
        {"DAT03", "forty"},
        {"DAT04", std::vector<std::any>{"num10", "num20", "42340"}},
        {"DAT05", std::vector<std::any>{32120, 2000, 3000}},
        {"DAT06", std::vector<std::any>{5110.2, 6110.3, 7110.4}},
    };
    m_data.saveManager.save("unittest-save2.sav", newSaveData);

    for (const auto& [key, val] : newSaveData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int loadSave(GlobalData& m_data)
{
    DataMap newLoadData;
    m_data.saveManager.load("unittest-save.sav", newLoadData);
    for (const auto& [key, val] : newLoadData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    DataMap newLoadData2;
    m_data.saveManager.load("unittest-save2.sav", newLoadData2);
    for (const auto& [key, val] : newLoadData2)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int saveConfig(GlobalData& m_data)
{
    m_data.saveManager.load("config.json", m_data.configData);
    m_data.saveManager.save("unittest-config.json", m_data.configData);

    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int modifyConfig(GlobalData& m_data)
{
    ConfigData configData;
    m_data.saveManager.load("unittest-config.json", m_data.configData);

    std::cout << "Before config file modification\n";
    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    m_data.configData[ConfigKey::NAME] = "unittest-config";
    m_data.saveManager.save("unittest-config.json", m_data.configData);
    m_data.saveManager.load("unittest-config.json", m_data.configData);

    std::cout << "\nAfter config file modification\n";
    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int loadConfig(GlobalData& m_data)
{
    m_data.saveManager.load("unittest-config.json", m_data.configData);
    for (const auto& [key, val] : m_data.configData)
    {
        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
    }

    return 1;
}

static int test_saveManager()
{
    GlobalData m_data;
    std::cout << "=========================================================\n";
    std::cout << "=== [Saving data to file] ===\n";
    if (saveData(m_data))
        std::cout << "Success\n\n";

    std::cout << "=== [Saving data to file 2] ===\n";
    if (saveData2(m_data))
        std::cout << "Success\n\n";

    std::cout << "=== [Loading data into memory] ===\n";
    if (loadSave(m_data))
        std::cout << "Success\n\n";

    std::cout << "=== [Saving config to file] ===\n";
    if (saveConfig(m_data))
        std::cout << "Success\n\n";

    std::cout << "=== [Modifying config and saving to file] ===\n";
    if (modifyConfig(m_data))
        std::cout << "Success\n\n";

    std::cout << "=== [Loading config into memory] ===\n";
    if (loadConfig(m_data))
        std::cout << "Success\n\n";

    return 0;
}