#pragma once

#include "../scenes/IScene.hpp"
#include <memory>
#include <stack>

class SceneManager
{
public:
    SceneManager();

    void addScene(std::unique_ptr<IScene> newScene, bool isReplacing = true);
    void removeScene();
    void processChange();
    IScene* getActiveScene();

private:
    std::stack<std::unique_ptr<IScene>> m_scenes;
    std::unique_ptr<IScene> m_newScene;
    bool m_removeFlag;
    bool m_addFlag;
    bool m_replaceFlag;
};