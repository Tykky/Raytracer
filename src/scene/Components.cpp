#include "Components.h"
#include <glm/glm.hpp>

struct TransformComponent
{
	glm::mat4 Transform;
	operator glm::mat4& () { return Transform; }
};
