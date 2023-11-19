#include "SceneManager.hpp"

SceneManager::SceneManager()
	: m_removeFlag(false), m_addFlag(false), m_replaceFlag(false)
{}

void SceneManager::addScene(std::unique_ptr<Scene> newScene, bool isReplacing)
{
	m_addFlag = true;
	m_replaceFlag = isReplacing;
	m_newScene = std::move(newScene);
}

void SceneManager::removeScene()
{
	m_removeFlag = true;
}

void SceneManager::processChange()
{
	if (m_removeFlag && !m_scenes.empty())
	{
		m_scenes.pop();
		if (!m_scenes.empty())
		{
			m_scenes.top()->resume();
		}
		m_removeFlag = false;
	}

	if (m_addFlag)
	{
		if (!m_scenes.empty())
		{
			if (m_replaceFlag)
			{
				m_scenes.pop();
			}
			else
			{
				m_scenes.top()->pause();
			}
		}
		m_scenes.push(std::move(m_newScene));
		m_scenes.top()->init();
		m_addFlag = false;
	}
}

std::unique_ptr<Scene>& SceneManager::getActiveScene()
{
	return m_scenes.top();
}
