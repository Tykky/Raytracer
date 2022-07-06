#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "Graphics.h"
#include "logging/Logging.h"
#include "ImFileDialog.h"
#include "core/Raytracer.h"
#include "editor/Types.h"
#include <tuple>

// Contains the GUI implementation for the editor

namespace Editor
{
    // Array of widget contexts
    template<typename WidgetContext>
    using WidgetArray = std::vector<WidgetContext>;

    // tuple of array of widget contexts
    template<typename... WidgetContexts>
    using WidgetArrayTuple = std::tuple<WidgetArray<WidgetContexts>...>;

    struct TextureViewerContext;
    struct LogViewerContext;
	struct ViewportContext;
    struct SettingsWidgetContext;
    struct WidgetInspectorContext;
    struct ImGuiMetricsWidgetContext;
    struct ImGuiStackToolWidgetContext;
    struct RTControlsContext;
    struct SystemInfoContext;
    struct RasterSettingsContext;

    // Each widget type has its own dynamic array to allow multiple instances
    typedef WidgetArrayTuple
	<
		TextureViewerContext,
		LogViewerContext,
		ViewportContext,
		SettingsWidgetContext,
		WidgetInspectorContext,
		ImGuiMetricsWidgetContext,
	    ImGuiStackToolWidgetContext,
		RTControlsContext,
		SystemInfoContext,
        RasterSettingsContext
	>
	WidgetStore;

    // WidgetStore helpers 

    template<typename Context>
    WidgetArray<Context>& getWidgetArray(WidgetStore& wStore)
    {
        return std::get<WidgetArray<Context>>(wStore);
    }

    template<typename Context>
    void insertWidgetArray(WidgetStore& wStore, std::string name)
    {
        auto& wArray = getWidgetArray<Context>(wStore);

        // Each widget type has seperate idx counter
        static unsigned long idx = 0;

        // Dear Imgui allows ids to be appended as suffix after "###"
        wArray.emplace_back(name, idx,  
		//  window title           id = name + idx
		//  <-->           <-------------------------->
            name + "###" + name + std::to_string(idx++)
		);
    }

    template<typename Context>
    void deleteMarkedWidgetsFromArray(WidgetArray<Context>& wArray)
    {
        auto isMarkedForDelete = [](Context& widget) 
        {
            return widget.markForDelete;
        };

		wArray.erase(
			std::remove_if(wArray.begin(), wArray.end(), isMarkedForDelete),
			wArray.end()
		);
    }

    void deleteAllMarkedWidgets(WidgetStore& wStore);

    // Widget Contexts

    // Common context for all the widgets
    struct WidgetContext
    {
        WidgetContext(const std::string& name, unsigned int id, const std::string& windowId) :
            name(name), id(id), windowId(windowId) {}

        std::string  name;
        unsigned int id = 0;
        std::string  windowId;
        bool         markForDelete = false;
    };

    struct TextureViewerContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;

        Vec2f         offset         = {0.0f, 0.0f};
        Vec2f         scale          = {600.0f, 600.0f};
		TextureStore* textureStore   = nullptr;
        Texture*      currentTexture = nullptr;
    };

    struct LogViewerContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;
        bool srollToBottom = true;
    };

    struct ViewportContext : public WidgetContext
    {
        ViewportContext(const std::string& name, unsigned int id, const std::string& windowId) :
                WidgetContext(name, id, windowId)
        {
            shaderProgram.addShader("data/shaders/vert.glsl", ShaderType::VERTEX);
            shaderProgram.addShader("data/shaders/frag.glsl", ShaderType::FRAGMENT);
            shaderProgram.link();
            framebuffer.addColorAttachment({3840, 2160, true});
            viewportTexture = reinterpret_cast<void*>(framebuffer.getColorAttachments()[0].id());
        }

	    bool          isPrimary    = false;
        bool          wireframe    = false;
        Vec2i         resolution   = { 3840, 2160 };
        Vec2f         offset       = { 0.0f, 0.0f };
        Vec2f         scale        = { 3840, 2160 };
        Camera        camera       = {static_cast<float>(resolution.x)/static_cast<float>(resolution.y),  // aspect ratio
                                     {0.0f, 0.0f, 3},   // pos
                                     {0.0, 0.0, -1}};   // target
        Vec2f           prevMousePos = { 0.0f, 0.0f };
        Framebuffer     framebuffer;
        Vertexbuffer    vertexbuffer = {defaultCubeData, sizeof(defaultCubeData)};
        ShaderProgram   shaderProgram;

        // Viewport shows this texture
        void* viewportTexture = nullptr;
    };

    struct SettingsWidgetContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;
    };

    struct WidgetInspectorContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;

        WidgetStore*  widgetStore = nullptr;
    };

    struct ImGuiMetricsWidgetContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;
    };

    struct ImGuiStackToolWidgetContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;
    };

    struct RTControlsContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;

        Raytracer*    raytracer       = nullptr;
        int           samples         = 100;
        WidgetStore*  widgetStore     = nullptr;
        TextureStore* textureStore    = nullptr;
        unsigned int  viewportTexture = 0;
    };

    struct SystemInfoContext : public WidgetContext
    {
        SystemInfoContext(const std::string& name, unsigned int id, const std::string& windowId) :
			WidgetContext(name, id ,windowId),
			GPUVendor(getGPUVendor()),
			renderer(getRenderer()),
			GLVersion(getGLSLVersion()),
			GLSLVersion(getGLSLVersion())
        {}

        std::string GPUVendor;
        std::string renderer;
        std::string GLVersion;
        std::string GLSLVersion;
    };

    struct RasterSettingsContext : public WidgetContext
    {
        using WidgetContext::WidgetContext;
    };

    // Draw widget functions
    // These allow multiple instances of each widget type.

    void drawWidget(WidgetStore* wStore, TextureViewerContext& ctx);
    void drawWidget(WidgetStore* wStore, LogViewerContext& ctx);
    void drawWidget(WidgetStore* wStore, ViewportContext& ctx);
    void drawWidget(WidgetStore* wStore, SettingsWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, WidgetInspectorContext& ctx);
    void drawWidget(WidgetStore* wStore, ImGuiMetricsWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, ImGuiStackToolWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, RTControlsContext& ctx);
    void drawWidget(WidgetStore* wStore, SystemInfoContext& ctx);
    void drawWidget(WidgetStore* wStore, RasterSettingsContext& ctx);

    template<typename Context>
    void drawWidgetArray(WidgetStore* wStore, WidgetArray<Context>& wArray)
    {
        for (auto& wCtx : wArray)
        {
            drawWidget(wStore, wCtx);
        }
    }

    void drawAllWidgets(WidgetStore* wStore);

    // Following widgets do not allow multiple instances

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore, bool enableMainWindowBorders);

    // Dockspace allows windows to be docked to the main window
    void drawDockspace(const char* name);
    void drawImFileDialogAndProcessInput();
    void drawTextureView(void* currentTexId, Vec2f& offset, Vec2f& scale);
    void drawTexturePickerComboBox(const char* preview, TextureStore* textureStore, Texture*& currentTexture);

    void moveTextureWhenDragged(float& offsetX, float& offsetY);
    void zoomTextureWhenScrolled(float& width, float& height);

    // Wrapper for Dear ImGui OpenGL backend implementation

    void ImGuiImplInitGLFW(void* window);
    void ImGuiImplInitGL3(const char* glslVer);
    void ImGuiImplGLNewFrame();
    void ImGuiImplGLFWNewFrame();
    void ImGuiRenderDrawData();
    void ImGuiRender();
}
#endif //RAYTRACER_WIDGETS_H