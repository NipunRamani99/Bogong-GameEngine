#pragma once
#include <glm/glm.hpp>
#include "Texture.h"
#include <unordered_map>
#include "ShaderManager.hpp"
namespace bogong {
	template<bool isTex>
	struct MaterialData {
		
	};
	template<>
	struct MaterialData<true> {
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		float shininess = 2.0f;
		void Bind(Program prog) {
			if (diffuse) {
				prog.setInt("material.diffuse", 0);
				glActiveTexture(GL_TEXTURE0);
				diffuse->Bind();

			}
			if (specular) {
				prog.setInt("material.specular", 1);
				glActiveTexture(GL_TEXTURE1);
				specular->Bind();
			}
		}
	};
	template<>
	struct MaterialData<false> {
		glm::vec3 ambient  = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 diffuse  = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
		float shininess	   = 2.0f;
		void Bind(Program prog) {
			prog.setVec3("material.ambient", ambient);
			prog.setVec3("material.diffuse", diffuse);
			prog.setVec3("material.specular", specular);
		}
	};

	class MaterialBank {
	private:
		
		std::unordered_map<size_t, MaterialData<true>> tex_mat_map;
		std::unordered_map<size_t, MaterialData<false>> colour_mat_map;
	public:
		
	};
}