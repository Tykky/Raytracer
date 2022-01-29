#include "editor/Editor.h"
#include "io/Io.h"
#include <iostream>

void test()
{
    Editor::Shader frag("frag.glsl", "../../src/editor/shaders/frag.glsl", Editor::ShaderType::FRAGMENT);
    Editor::Shader vert("vert.glsl", "../../src/editor/shaders/vert.glsl", Editor::ShaderType::VERTEX);

    Editor::ShaderStore shaderStore;
    shaderStore.push_back(std::move(frag));
    shaderStore.push_back(std::move(vert));

    Editor::ShaderProgram program = {&shaderStore};
}

int main()
{
    Editor::Options options;

    options.enableViewports = true;
    GLFWwindow* window = Editor::createWindow("kebab", 800, 800, options);
    Editor::WidgetStore widgetStore;
    Editor::TextureStore textureStore;

    if (auto tex = Editor::loadTexture("scot.png"))
        textureStore.push_back(std::move(tex.value()));

    Editor::init(window, options, widgetStore, textureStore);
    test();
    Editor::renderLoop(window, widgetStore, textureStore);
    Editor::destroyWindow(window);
}