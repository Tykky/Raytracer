#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <cstdint>
#include <entt/entt.hpp>
#include <vector>
#include "scene/Components.h"

namespace RT
{
    struct Scene
    {
        entt::registry registry;
    };

    template<typename... Components>
    void addComponents(Scene& scene, entt::entity entity)
    {
        (scene.registry.emplace<Components>(entity), ...);
    }
}

#endif // RAYTRACER_SCENE_H