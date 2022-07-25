#include "scene/Scene.h"

namespace RT
{
    Entity addEntity(Scene& scene)
    {
        return static_cast<std::uint32_t>(scene.registry.create());
    }

    void destroyEntity(Scene& scene, Entity entity)
    {
        scene.registry.destroy(static_cast<entt::entity>(entity));
    }

    void destroyAllEntities(Scene& scene)
    {
        scene.registry.clear();
    }
}