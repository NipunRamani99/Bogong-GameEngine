#pragma once
#include <glm/glm.hpp>
namespace bogong {
	namespace node {
		/*
			root_node = std::make_shared<Node>();
			//Fill up pointlight;
			light = std::make_shared<PointLightNode>( PointLight );
			root_node->addNode( light );
		*/
		struct PointLight {
			glm::vec3 pos = glm::vec3(0.0f);
			glm::vec3 ambient = glm::vec3(0.0f);
			glm::vec3 diffuse = glm::vec3(0.0f);
			glm::vec3 specular = glm::vec3(0.0f);
			float constant = 1.0f;
			float linear = 0.09f;
			float quadratic = 0.032f;
		};
		struct SpotLight {
			glm::vec3 pos = glm::vec3(0.0f);
			glm::vec3 dir = glm::vec3(0.0f);
			glm::vec3 ambient = glm::vec3(0.0f);
			glm::vec3 diffuse = glm::vec3(0.0f);
			glm::vec3 specular = glm::vec3(0.0f);
			float innercutoff = 12.5f;
			float outercutoff = 15.0f;
			float constant = 1.0f;
			float linear = 0.09f;
			float quadratic = 0.032f;
		};
		struct DirectionalLight {
			glm::vec3 dir = glm::vec3(0.0f);
			glm::vec3 ambient = glm::vec3(0.0f);
			glm::vec3 diffuse = glm::vec3(0.0f);
			glm::vec3 specular = glm::vec3(0.0f);
		};
	}
}