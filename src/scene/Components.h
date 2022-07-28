#ifndef RAYTRACER_COMPONENTS_H
#define RAYTRACER_COMPONENTS_H

#include "util/Types.h"
#include <cstddef>
#include <cstdint>
#include <utility>
#include "mesh/Mesh.h"
#include "util/Math.h"

namespace RT
{
	// Components used by ECS
	struct TransformComponent
	{
		v3f translation;
		v3f rotation;
		v3f scale;
	};

	struct CameraComponent
	{
		float   fov;
		float   zNear;
		float   zFar;
		float   aspectRatio;

		float   yaw;
		float   pitch;

		v3f   dir;
		v3f   right;
		v3f   up;

		v3f   target;

		m44f view;
		m44f projection;
	};

	struct MeshComponent
	{
		float*       vertices;
		u32          size;
		u32          meshId;
		VertexLayout layout;
	};
}

#endif // RAYTRACER_COMPONENTS_H