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

    // Forward declaration of widget contexts

    struct TextureViewerContext;
    struct LogViewerContext;
	struct ViewportContext;
    struct SettingsWidgetContext;
    struct WidgetInspectorContext;
    struct ImGuiMetricsWidgetContext;
    struct ImGuiStackToolWidgetContext;
    struct RTControlsContext;
    struct SystemInfoContext;

    // Each widget type has its own array to allow multiple instances
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
		SystemInfoContext
	>
	WidgetStore;

    // WidgetStore helper functions

    template<typename Context>
    WidgetArray<Context>& getWidgetArray(WidgetStore& wStore)
    {
        return std::get<WidgetArray<Context>>(wStore);
    }

    template<typename Context>
    void insertWidgetArray(WidgetStore& wStore, std::string name)
    {
        auto& wArray = getWidgetArray<Context>(wStore);
        static unsigned int idx = 0;
        wArray.push_back({
            name,
            idx,
            {name + "###" + std::to_string(idx++)}
		});
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

    // Common context for all widgets
    struct WidgetContext
    {
        std::string  name;
        unsigned int id = 0;
        std::string  windowId;
        bool         markForDelete = false;
    };

    struct TextureViewerContext
    {
        WidgetContext wCtx;
        Vec2f         offset         = {0.0f, 0.0f};
        Vec2f         scale          = {600.0f, 600.0f};
		TextureStore* textureStore   = nullptr;
        Texture*      currentTexture = nullptr;
    };

    struct LogViewerContext
    {
        WidgetContext   wCtx;
        bool srollToBottom = true;
    };

    struct ViewportContext
    {
        WidgetContext wCtx;
	    bool          isPrimary    = false;
        bool          wireframe    = false;
        Vec2i         resolution   = { 1920, 1080 };
        Vec2f         offset       = { 0.0f, 0.0f };
        Vec2f         scale        = { 1920, 1080 };
        Camera        camera       = {static_cast<float>(resolution.x)/
                                      static_cast<float>(resolution.y),  // aspect ratio
                                     {0, 0.0, 3},     // pos
                                     {0.0, 0.0, -1}}; // target
        Vec2f         prevMousePos = { 0.0f, 0.0f };
        Framebuffer   framebuffer;
        Vertexbuffer  vertexbuffer;
        ShaderProgram shaderProgram;

        // Viewport shows this texture
        void* m_viewportTexture = nullptr;
    };

    struct SettingsWidgetContext 
    {
        WidgetContext wCtx;
    };

    struct WidgetInspectorContext
    {
        WidgetContext wCtx;
        WidgetStore*  widgetStore;
    };

    struct ImGuiMetricsWidgetContext 
    {
        WidgetContext wCtx;
    };

    struct ImGuiStackToolWidgetContext 
    {
        WidgetContext wCtx;
    };

    struct RTControlsContext
    {
        WidgetContext wCtx;
        Raytracer*    raytracer;
        int           samples = 100;
        WidgetStore*  widgetStore;
        TextureStore* textureStore;
        unsigned int  viewportTexture = 0;
    };

    struct SystemInfoContext
    {
        WidgetContext wCtx;
        std::string   GPUVendor;
        std::string   renderer;
        std::string   GLVersion;
        std::string   GLSLVersion;
    };

    // Draw widget functions

    void drawWidget(WidgetStore* wStore, TextureViewerContext& ctx);
    void drawWidget(WidgetStore* wStore, LogViewerContext& ctx);
    void drawWidget(WidgetStore* wStore, ViewportContext& ctx);
    void drawWidget(WidgetStore* wStore, SettingsWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, WidgetInspectorContext& ctx);
    void drawWidget(WidgetStore* wStore, ImGuiMetricsWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, ImGuiStackToolWidgetContext& ctx);
    void drawWidget(WidgetStore* wStore, RTControlsContext& ctx);
    void drawWidget(WidgetStore* wStore, SystemInfoContext& ctx);

    template<typename Context>
    void drawWidgetArray(WidgetStore* wStore, WidgetArray<Context>& wArray)
    {
        for (auto& wCtx : wArray)
        {
            drawWidget(wStore, wCtx);
        }
    }

    void drawAllWidgets(WidgetStore* wStore);

    // Shows the main menubar at the top of the main window
    void drawMainMenuBar(WidgetStore& widgetStore, TextureStore& textureStore);
    // Dockspace simply allows windows to be docked to the main window
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