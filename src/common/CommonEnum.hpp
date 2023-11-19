#pragma once

#include <any>
#include <atomic>
#include <string>
#include <unordered_map>

enum class SystemStatus
{
	OK,
	ERROR,
	FILE_MNGR_SUCCESS,
	FILE_MNGR_CREATED_FILE,
	FILE_MNGR_FAIL_WRITE,
	FILE_MNGR_FAIL_READ,
	FILE_MNGR_FAIL_CREATE_FOLDER,
	FILE_MNGR_FAIL_LOOKUP,
	FILE_MNGR_FAIL_FILE_EMPTY,
};

enum class SystemID
{
	DEFAULT,
	SCENE_MNGR,
	WAYPOINT_MNGR,
};

enum class PathID
{
	DEFAULT,
	CONFIG,
	SAVE,
	SCRIPT
};

enum ConfigKey
{
	NAME,
	VERSION,
	PATH_CONFIG,
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

inline const std::string ssToStr(SystemStatus status)
{
	switch (status)
	{
	case SystemStatus::OK:
		return "OK";
	case SystemStatus::ERROR:
		return "ERROR";
	case SystemStatus::FILE_MNGR_SUCCESS:
		return "FILE_MNGR_SUCCESS";
	case SystemStatus::FILE_MNGR_CREATED_FILE:
		return "FILE_MNGR_CREATED_FILE";
	case SystemStatus::FILE_MNGR_FAIL_WRITE:
		return "FILE_MNGR_FAIL_WRITE";
	case SystemStatus::FILE_MNGR_FAIL_READ:
		return "FILE_MNGR_FAIL_READ";
	case SystemStatus::FILE_MNGR_FAIL_CREATE_FOLDER:
		return "FILE_MNGR_FAIL_CREATE_FOLDER";
	case SystemStatus::FILE_MNGR_FAIL_LOOKUP:
		return "FILE_MNGR_FAIL_LOOKUP";
	default:
		return "DEFAULT";
	}
}

inline const std::string sidToStr(const SystemID sysId)
{
	switch (sysId)
	{
	case SystemID::SCENE_MNGR:
		return "SCENE_MNGR";
	case SystemID::WAYPOINT_MNGR:
		return "WAYPOINT_MNGR";
	default:
		return "DEFAULT";
	}
}

inline const std::string pidToStr(const PathID pathId)
{
	switch (pathId)
	{
	case PathID::CONFIG:
		return "CONFIG";
	case PathID::SAVE:
		return "SAVE";
	case PathID::SCRIPT:
		return "SCRIPT";
	default:
		return "DEFAULT";
	}
}

inline const PathID strToPid(const std::string path)
{
	if (path == "CONFIG")
		return PathID::CONFIG;
	else if (path == "SAVE")
		return PathID::SAVE;
	else if (path == "SCRIPT")
		return PathID::SCRIPT;
	else 
		return PathID::DEFAULT;
}

inline std::string anyToString(const std::any& val)
{
	if (auto x = std::any_cast<std::string>(&val))
		return *x;
	if (auto x = std::any_cast<const char*>(&val))
		return *x;
	else if (auto x = std::any_cast<bool>(&val))
		return (std::to_string(*x) == "1") ? "true" : "false";
	else if (auto x = std::any_cast<int>(&val))
		return std::to_string(*x);
	else if (auto x = std::any_cast<float>(&val))
		return std::to_string(*x);
	else if (auto x = std::any_cast<double>(&val))
		return std::to_string(*x);
	return "";
}

typedef std::unordered_map<ConfigKey, std::any> ConfigMap;