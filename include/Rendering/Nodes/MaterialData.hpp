#pragma once
#include <glm/glm.hpp>
namespace bogong {
	namespace node {
		struct MaterialData {
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		};
	}
}