#include "Mesh.h"

namespace RT
{
    static const std::unordered_map<VertexLayout, std::size_t> numElementsMap = {
        {VertexLayout::POINT_NORMAL_UV, 8},
        {VertexLayout::POINT_NORMAL,    6},
        {VertexLayout::POINT_UV,        5},
        {VertexLayout::POINT,           3}
    };

    std::size_t numelementsVertexLayout(VertexLayout layout)
    {
        return numElementsMap.at(layout);
    }
}