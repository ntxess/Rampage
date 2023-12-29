#pragma once

#include "../common/Scene.hpp"
#include <memory>
#include <stack>

class SceneManager
{
public:
	SceneManager();

	void addScene(std::unique_ptr<Scene> newScene, bool isReplacing = true);
	void removeScene();
	void processChange();
	Scene* getActiveScene();

private:
	std::stack<std::unique_ptr<Scene>> m_scenes;
	std::unique_ptr<Scene> m_newScene;
	bool m_removeFlag;
	bool m_addFlag;
	bool m_replaceFlag;
};