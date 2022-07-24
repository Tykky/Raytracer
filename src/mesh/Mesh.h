#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <cstddef>
#include <unordered_map>

namespace RT
{
    enum class VertexLayout
    {
        POINT_NORMAL_UV,
        POINT_NORMAL,
        POINT_UV,
        POINT
    };

    // Number of elements per layout (e.g number of floats)
    std::size_t numElementsInVertexLayout(VertexLayout layout);

    template<typename T>
    std::size_t sizeofVertexLayout(VertexLayout layout)
    {
        return sizeof(T) * numElementsVertexLayout(layout);
    }
}

#endif // RAYTRACER_MESH_H