#pragma once

class Sandbox;
class MainMenu;
class GameOfLifeSim;
class Editor;

class ISceneVisitor
{
public:
	virtual ~ISceneVisitor() = default;
	virtual void visit(const Sandbox* sandbox) = 0;
	virtual void visit(const MainMenu* mainMenu) = 0;
	virtual void visit(const GameOfLifeSim* gameOfLifeSim) = 0;
	virtual void visit(const Editor* editor) = 0;
};