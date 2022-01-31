#include "editor/Editor.h"
#include "io/Io.h"
#include <iostream>

void test()
{
    Editor::ShaderProgram shader;
    shader.addShader("vert.glsl",Editor::ShaderType::VERTEX);
    shader.addShader("frag.glsl", Editor::ShaderType::FRAGMENT);
    shader.link();
    shader.use();
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