#pragma once

#include <entt/entt.hpp>
#include <any>
#include <atomic>
#include <string>
#include <string_view>
#include <unordered_map>

constexpr std::string_view RESET("\033[0m");    // Color Reset 
constexpr std::string_view RED("\033[31m");     // Main Thread
constexpr std::string_view GREEN("\033[32m");   // Physic Thread 
constexpr std::string_view YELLOW("\033[33m");	// Render Thread
constexpr std::string_view BLUE("\033[34m");	// Audio Thread
constexpr std::string_view MAGENTA("\033[35m");	// Resource Thread
constexpr std::string_view CYAN("\033[36m");	// Debug/Logging Thread

/**
 * @brief System Status codes.
*/
enum class SystemStatus
{
    SUCCESS,
    ERROR,
    SAVE_MNGR_SUCCESS,
    SAVE_MNGR_CREATED_FILE,
    SAVE_MNGR_FAIL_WRITE,
    SAVE_MNGR_FAIL_READ,
    SAVE_MNGR_FAIL_FILE_EMPTY,
    RSRC_MNGR_SUCCESS,
    RSRC_MNGR_FAIL_LOAD,
    RSRC_MNGR_FAIL_UNLOAD,

};

/**
 * @brief Enum key for configurations values.
*/
enum ConfigKey
{
    NAME,
    VERSION,
    DEBUG_MODE,
    DEBUG_LOG_FILTER_SEVERITY,
    CONFIG_FOLDER,
    DEBUG_LOG_FOLDER,
    SAVE_FOLDER,
    SCRIPT_FOLDER,
    WIDTH,
    HEIGHT,
    FRAMERATE,
    FULLSCREEN,
    DMG_NUMBER,
    VISUAL_EFFECTS,
    SCREEN_SHAKE,
    MATURE,
    MUSIC_VOL,
    EFFECTS_VOL,
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_JUMP,
    KEY_EXIT,
    KEY_ENTER,
    KEY_BACK,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    DUB,
    SUB,
    SUBTITLE,
    COLOR_BLIND
};

/**
 * @brief [FOR DEBUG] SystemStatus enum to string for debugging.
 * @param [IN] status
 * @return SystemStatus in std::string_view.
*/
inline constexpr std::string_view ssToStr(SystemStatus status)
{
    switch (status)
    {
    case SystemStatus::SUCCESS:
        return "SUCCESS";
    case SystemStatus::ERROR:
        return "ERROR";
    case SystemStatus::SAVE_MNGR_SUCCESS:
        return "SAVE_MNGR_SUCCESS";
    case SystemStatus::SAVE_MNGR_CREATED_FILE:
        return "SAVE_MNGR_CREATED_FILE";
    case SystemStatus::SAVE_MNGR_FAIL_WRITE:
        return "SAVE_MNGR_FAIL_WRITE";
    case SystemStatus::SAVE_MNGR_FAIL_READ:
        return "SAVE_MNGR_FAIL_READ";
    case SystemStatus::SAVE_MNGR_FAIL_FILE_EMPTY:
        return "SAVE_MNGR_FAIL_FILE_EMPTY";
    default:
        return "DEFAULT";
    }
}

/**
 * @brief [FOR DEBUG] Converts std::any value to std::string.
 * @param [IN] val
 * @return std::any in std::string.
*/
inline std::string anyToString(const std::any& val)
{
    if (auto x = std::any_cast<std::string>(&val))
        return *x;
    else if (auto x = std::any_cast<const char*>(&val))
        return *x;
    else if (auto x = std::any_cast<bool>(&val))
        return (std::to_string(*x) == "1") ? "true" : "false";
    else if (auto x = std::any_cast<int>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<float>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<double>(&val))
        return std::to_string(*x);
    else if (auto x = std::any_cast<std::vector<std::any>>(&val))
    {
        std::string result = "[";
        for (size_t i = 0; i < x->size(); ++i)
        {
            result += anyToString(x->at(i));
            if (i + 1 < x->size())
                result += ", ";
        }
        result.push_back(']');
        return result;
    }
    return "";
}

typedef entt::hashed_string::hash_type HashKey;
typedef std::unordered_map<ConfigKey, std::any> ConfigData;
typedef std::unordered_map<std::string, std::any> DataMap;
typedef std::unordered_map<HashKey, std::unique_ptr<DataMap>> SaveData;