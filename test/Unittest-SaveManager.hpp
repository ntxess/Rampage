//#include "../src/core/ApplicationContext.hpp"
//#include "../src/core/util/AnyToString.hpp"
//#include <any>
//#include <iostream>
//#include <string>
//#include <unordered_map>
//
//static int saveData(ApplicationContext& m_appContext)
//{
//    m_appContext.saveData[entt::hashed_string("unittest-save")] = std::make_unique<std::unordered_map<std::string, std::any>>();
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT00", false);
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT01", 1);
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT02", 2);
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT03", "four");
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT04", std::vector<std::any>{"num1", "num2", "num3"});
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT05", std::vector<std::any>{100, 200, 300});
//    m_appContext.saveData[entt::hashed_string("unittest-save")]->emplace("DAT06", std::vector<std::any>{511.2, 611.3, 711.4});
//    m_appContext.saveManager.save("save/unittest-save.sav", *(m_appContext.saveData[entt::hashed_string("unittest-save")]));
//
//    std::unordered_map<std::string, std::any> loadedSaveData;
//    m_appContext.saveManager.load("save/unittest-save.sav", loadedSaveData);
//
//    int size = 0;
//    for (const auto& [key, val] : *(m_appContext.saveData[entt::hashed_string("unittest-save")]))
//    {
//        auto saved = anyToString(val);
//        auto loaded = anyToString(loadedSaveData.at(key));
//        std::cout << key << "[" << saved << "]";
//        if (saved == loaded)
//        {
//            std::cout << " == ";
//            ++size;
//        }
//        else
//        {
//            std::cout << " != ";
//        }
//        std::cout << key << "[" << loaded << "]\n";
//    }
//
//    return size == loadedSaveData.size();
//}
//
//static int saveData2(ApplicationContext& m_appContext)
//{
//    std::unordered_map<std::string, std::any> newSaveData = {
//        {"DAT00", true},
//        {"DAT01", 10},
//        {"DAT02", 20},
//        {"DAT03", "forty"},
//        {"DAT04", std::vector<std::any>{"num10", "num20", "42340"}},
//        {"DAT05", std::vector<std::any>{32120, 2000, 3000}},
//        {"DAT06", std::vector<std::any>{5110.2, 6110.3, 7110.4}},
//    };
//    m_appContext.saveManager.save("save/unittest-save2.sav", newSaveData);
//
//    std::unordered_map<std::string, std::any> loadedSaveData;
//    m_appContext.saveManager.load("save/unittest-save2.sav", loadedSaveData);
//
//    int size = 0;
//    for (const auto& [key, val] : newSaveData)
//    {
//        auto saved = anyToString(val);
//        auto loaded = anyToString(loadedSaveData.at(key));
//        std::cout << key << "[" << saved << "]";
//        if (saved == loaded)
//        {
//            std::cout << " == ";
//            ++size;
//        }
//        else
//        {
//            std::cout << " != ";
//        }
//        std::cout << key << "[" << loaded << "]\n";
//    }
//
//    return size == loadedSaveData.size();
//}
//
//static int saveConfig(ApplicationContext& m_appContext)
//{
//    m_appContext.saveManager.load("config.json", m_appContext.configData);
//    m_appContext.saveManager.save("unittest-config.json", m_appContext.configData);
//
//    ConfigData loadConfigData;
//    m_appContext.saveManager.load("unittest-config.json", loadConfigData);
//
//    int size = 0;
//    for (const auto& [key, val] : m_appContext.configData)
//    {
//        auto saved = anyToString(val);
//        auto loaded = anyToString(loadConfigData.at(key));
//        std::cout << key << "[" << saved << "]";
//        if (saved == loaded)
//        {
//            std::cout << " == ";
//            ++size;
//        }
//        else
//        {
//            std::cout << " != ";
//        }
//        std::cout << key << "[" << loaded << "]\n";
//    }
//
//    return size == loadConfigData.size();
//}
//
//static int modifyConfig(ApplicationContext& m_appContext)
//{
//    m_appContext.configDataSerializer.load("config.json", m_appContext.configData);
//    m_appContext.configDataSerializer.save("unittest-config.json", m_appContext.configData);
//
//    std::cout << "Before config file modification\n";
//    for (const auto& [key, val] : m_appContext.configData.data())
//    {
//        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
//    }
//
//    m_appContext.configData.data()["name"] = "unittest-config";
//    m_appContext.configDataSerializer.save("unittest-config.json", m_appContext.configData);
//    m_appContext.configDataSerializer.load("config.json", m_appContext.configData);
//
//    DataStore loadConfigData;
//    m_appContext.configDataSerializer.load("unittest-config.json", loadConfigData);
//
//    std::cout << "\nAfter config file modification\n";
//    for (const auto& [key, val] : loadConfigData.data())
//    {
//        std::cout << "Key: " << key << " | Val: " << anyToString(val) << "\n";
//    }
//
//    return anyToString(m_appContext.configData.data()["name"]) != anyToString(loadConfigData.data()["name"]);
//}
//
//static void viewer(ApplicationContext& m_appContext)
//{
//    m_appContext.saveManager.load("config.json", m_appContext.configData);
//
//    for (const auto& [key, val] : m_appContext.configData)
//    {
//        std::cout << "Key: " << key << " | Val: " << anyToString(val) << " | Type: " << m_appContext.saveManager.getType(val) << "\n";
//    }
//}
//
//static int test_saveManager()
//{
//    ApplicationContext m_appContext;
//    std::cout << "=========================================================\n";
//    std::cout << "=== [Saving data to file] ===\n";
//    if (saveData(m_appContext))
//        std::cout << "Success\n\n";
//    else
//        std::cout << "Failed\n\n";
//
//    std::cout << "=== [Saving data to file 2] ===\n";
//    if (saveData2(m_appContext))
//        std::cout << "Success\n\n";
//    else
//        std::cout << "Failed\n\n";
//
//    std::cout << "=== [Saving config to file] ===\n";
//    if (saveConfig(m_appContext))
//        std::cout << "Success\n\n";
//    else
//        std::cout << "Failed\n\n";
//
//    std::cout << "=== [Modifying config and saving to file] ===\n";
//    if (modifyConfig(m_appContext))
//        std::cout << "Success\n\n";
//    else
//        std::cout << "Failed\n\n";
//
//    std::cout << "=== [Reading config file] ===\n";
//    viewer(m_appContext);
//
//    return 0;
//}