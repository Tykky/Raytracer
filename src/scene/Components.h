#ifndef RAYTRACER_COMPONENTS_H
#define RAYTRACER_COMPONENTS_H

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct TransformComponent
{
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 0.0f, 0.0f, 0.0f };

	TransformComponent(const glm::vec3& translation) :
		Translation(translation) {}

	glm::mat4 getTransform()
	{
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 }) *
						glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 }) *
						glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

		return glm::translate(glm::mat4(1.0f), Translation) *
			   rot *
			   glm::scale(glm::mat4(1.0f), Scale);
			   
	}
};

#endif // RAYTRACER_COMPONENTS_H
