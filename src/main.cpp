#include "core/Engine.hpp"
#include "editor/Editor.hpp"
#include <memory>

int main()
{
	Engine engine("config/config.json", std::make_unique<Editor>());
	engine.run();

	return 0;
}