include(FetchContent)

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG c9461a9b55ba954df0489bab6420eb297bed846b
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 97da62a027794d9ff0f4512268cb9a73a8fb5073
)

FetchContent_Declare(
    glew
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

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG 2f2d04b3e840428a18942ca2d3d65203ec564647
)

FetchContent_Declare(
    simplejson
    GIT_REPOSITORY https://github.com/nbsdx/SimpleJSON.git
    GIT_TAG 8dd3e9b84a0b532c46f22f402bafb2c4cf8621eb
)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG 28aa35160de343a55e95ddd685286af68997cccd
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG bc654faf82e54d29f6c5fd7efcfd16a587917e82
)

# Google benchmark requires gtest
FetchContent_MakeAvailable(
    gtest
)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG 2a78e8cbe9b104834d96c78ccc9f9513a29f8c71
)

FetchContent_MakeAvailable(
   glfw
   glew
   glm
   imgui
   tinyobjloader
   spdlog
   simplejson
   entt
   fmt
   benchmark
)

find_package(OpenGL REQUIRED)

include_directories(external/stb_image)
include_directories(external/ImFileDialog)

include_directories(${tinyobjloader_SOURCE_DIR})
include_directories(${glm_SOURCE_DIR})
include_directories(${glew_SOURCE_DIR}/include)
include_directories(${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/backends)
include_directories(${spdlog_SOURCE_DIR}/include)
include_directories(${Simplejson_SOURCE_DIR})
include_directories(${entt_SOURCE_DIR}/single_include)
include_directories(${fmt_SOURCE_DIR}/include)
include_directories(${benchmark_SOURCE_DIR}/include)

set(EXTERNAL
    OpenGL::GL
    libglew_static
    glfw
)

set(DEAR_IMGUI_SOURCE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)