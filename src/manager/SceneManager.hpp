#pragma once

#include "../common/Scene.hpp"
#include <memory>
#include <stack>

class SceneManager
{
private:
	std::stack<std::unique_ptr<Scene>> m_scenes;
	std::unique_ptr<Scene> m_newScene;
	bool m_removeFlag;
	bool m_addFlag;
	bool m_replaceFlag;

public:
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	~SceneManager() = default;

	void addScene(std::unique_ptr<Scene> newScene, bool isReplacing = true);
	void removeScene();
	void processChange();
	Scene* getActiveScene();
};