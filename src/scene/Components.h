#ifndef RAYTRACER_COMPONENTS_H
#define RAYTRACER_COMPONENTS_H

#include "types/Types.h"
#include <utility>
#include "mesh/Mesh.h"

namespace RT
{
	// Components used by ECS
	struct TransformComponent
	{
		Vec3f translation;
		Vec3f rotation;
		Vec3f scale;
	};

	struct CameraComponent
	{
		float   fov;
		float   zNear;
		float   zFar;
		float   aspectRatio;

		float   yaw;
		float   pitch;

		Vec3f   dir;
		Vec3f   right;
		Vec3f   up;

		Vec3f   target;

		Mat4x4f view;
		Mat4x4f projection;
	};

	struct MeshComponent
	{
		float*       data;
		VertexLayout layout;
	};

}

#endif // RAYTRACER_COMPONENTS_H