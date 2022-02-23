include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG c9461a9b55ba954df0489bab6420eb297bed846b
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 97da62a027794d9ff0f4512268cb9a73a8fb5073
)

FetchContent_Declare(
    glew-cmake
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG 7c2b7514f64234756f7df051b9316f1e46f5a6e7
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 6ad79aae3eb5bf809c30bf1168171e9e55857e45
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 994853511803cd37ccb0e804442049c947b09cc3
)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG 8322e00ae685ea623ab6ac5a6cebcfa2d22fbf93
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glew-cmake)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(imgui)
FetchContent_MakeAvailable(tinyobjloader)

find_package(OpenGL REQUIRED)

include_directories(external/stb_image)
include_directories(external/ImFileDialog)

include_directories(${tinyobjloader_SOURCE_DIR})
include_directories(${glm_SOURCE_DIR})
include_directories(${glew-cmake_SOURCE_DIR}/include)
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(external)

set(DEAR_IMGUI_SOURCE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)