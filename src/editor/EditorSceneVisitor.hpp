#pragma once

#include "../core/interface/ISceneVisitor.hpp"
#include "../scene/Scenes.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

class EditorSceneVisitor : public ISceneVisitor
{
public:
    void visit(const Sandbox* sandbox) override;
    void visit(const MainMenu* mainMenu) override;
    void visit(const GameOfLifeSim* gameOfLifeSim) override;
    void visit(const Editor* editor) override;
};