#include "core/Engine.hpp"
#include "editor/Editor.hpp"

// #include "../test/Unittest-SaveManager.hpp"

int main()
{
    Engine engine(std::make_unique<Editor>());
    engine.run();

    // test_saveManager();
    return 0;
}