#include "Mesh.h"

namespace RT
{
    static const std::unordered_map<VertexLayout, std::size_t> numElementsMap = {
        {VertexLayout::POINT_NORMAL_UV, 8},
        {VertexLayout::POINT_NORMAL,    6},
        {VertexLayout::POINT_UV,        5},
        {VertexLayout::POINT,           3}
    };

    u32 numelementsInVertexLayout(VertexLayout layout)
    {
        switch (layout)
        {
            case VertexLayout::POINT_NORMAL_UV:
                return 8;
            case VertexLayout::POINT_NORMAL:
                return 6;
            case VertexLayout::POINT_UV:
                return 5;
            case VertexLayout::POINT:
                return 3;
            default:
                return 0;
        }
    }
}