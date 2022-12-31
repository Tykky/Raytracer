#ifndef RAYTRACER_WIDGETS_H
#define RAYTRACER_WIDGETS_H

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "Graphics.h"
#include "logging/Logging.h"
#include "util/Types.h"
#include <tuple>
#include "util/Math.h"

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

        // Dear Imgui allows id:s to be appended as suffix after "###"
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
            return widget.wCtx.markForDelete;
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
        std::string  name;
        unsigned int id;
        std::string  windowId;
        bool         markForDelete = false;
    };

    struct TextureViewerContext
    {
        TextureViewerContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
        v2f           offset         = {0.0f, 0.0f};
        v2f           scale          = {600.0f, 600.0f};
		TextureStore* textureStore   = nullptr;
        Texture*      currentTexture = nullptr;
    };

    struct LogViewerContext
    {
        LogViewerContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx = {};
        bool srollToBottom = true;
    };

    struct ViewportContext
    {
        WidgetContext wCtx;
        ViewportContext(std::string name, unsigned int id, std::string windowId)
        {
            shaderProgram.addShader("data/shaders/vert.glsl", ShaderType::VERTEX);
            shaderProgram.addShader("data/shaders/frag.glsl", ShaderType::FRAGMENT);
            shaderProgram.link();
            framebuffer.addColorAttachment({3840, 2160, true});
            viewportTexture = reinterpret_cast<void*>(framebuffer.getColorAttachments()[0].id());

            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        }

	    bool          isPrimary    = false;
        bool          wireframe    = false;
        v2i32         resolution   = { 3840, 2160 };
        v2f           offset       = { 0.0f, 0.0f };
        v2f           scale        = { 3480, 2160 };
        Camera        camera       = {static_cast<float>(resolution.x())/
									  static_cast<float>(resolution.y()),  // aspect ratio
                                     {0.0f, 0.0f, 3},   // pos
                                     {0.0, 0.0, -1}};   // target
        v2f           prevMousePos = { 0.0f, 0.0f };
        Framebuffer   framebuffer;
        Vertexbuffer  vertexbuffer = {defaultCubeData, sizeof(defaultCubeData)};
        ShaderProgram shaderProgram;

        // Viewport shows this texture
        void* viewportTexture = nullptr;
    };

    struct SettingsWidgetContext
    {
        SettingsWidgetContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
    };

    struct WidgetInspectorContext
    {
        WidgetInspectorContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
        WidgetStore*  widgetStore = nullptr;
    };

    struct ImGuiMetricsWidgetContext
    {
        ImGuiMetricsWidgetContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
    };

    struct ImGuiStackToolWidgetContext
    {
        ImGuiStackToolWidgetContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
    };

    struct RTControlsContext
    {
        RTControlsContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
        int           samples         = 100;
        WidgetStore*  widgetStore     = nullptr;
        TextureStore* textureStore    = nullptr;
        unsigned int  viewportTexture = 0;
    };

    struct SystemInfoContext
    {
        SystemInfoContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
        std::string GPUVendor;
        std::string renderer;
        std::string GLVersion;
        std::string GLSLVersion;
    };

    struct RasterSettingsContext
    {
        RasterSettingsContext(std::string name, unsigned int id, std::string windowId)
        {
            wCtx.name = name;
            wCtx.id = id;
            wCtx.windowId = windowId;
        };

        WidgetContext wCtx;
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
    void drawMainMenuBar(WidgetStore& widgetStore, bool enableMainWindowBorders);

    // Dockspace allows windows to be docked to the main window
    void drawDockspace(const char* name);
    void drawImFileDialogAndProcessInput();
    void drawTextureView(void* currentTexId, v2f& offset, v2f& scale);
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