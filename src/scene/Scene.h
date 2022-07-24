#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <cstdint>
#include <entt/entt.hpp>
#include <vector>

namespace RT
{
    typedef std::uint32_t Entity;
    typedef entt::entity enttEntity;

    struct Scene
    {
        entt::registry registry;
    };

    Entity addEntity(Scene& scene);
    void destroyEntity(Scene& scene, Entity entity);
    void destroyAllEntities(Scene& scene);

    template<typename Component, typename... Args>
    void addComponent(Scene& scene, Entity entity, Args&&... args)
    {
        scene.registry.emplace<Component>(static_cast<enttEntity>(entity), std::forward<Args>(args)...);
    }

    template<typename Component>
    void eraseComponent(Scene& scene, Entity entity)
    {
        scene.registry.erase<Component>(static_cast<enttEntity>(entity));
    }

    template<typename Component>
    void eraseAllComponents(Scene& scene)
    {
        scene.registry.clear<Component>();
    }

    template<typename Component, typename... Other, typename... Exclude>
    entt::basic_view<entt::entity, entt::get_t<Component, Other...>, entt::exclude_t<Exclude...>> sceneView(Scene& scene)
    {
        return scene.registry.view<Component, Other..., Exclude...>();
    }

    template<typename Component>
    Component& getComponent(Scene& scene, Entity entity)
    {
        return scene.registry.get<Component>(static_cast<enttEntity>(entity));
    }
}
#endif // RAYTRACER_SCENE_H