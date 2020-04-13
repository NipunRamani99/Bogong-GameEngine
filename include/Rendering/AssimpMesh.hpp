#pragma once
#include "Mesh.hpp"
namespace bogong {
	struct MeshVertex {
		glm::vec3 pos;
		glm::vec3 norms;
		glm::vec2 uv;
	};
	class AssimpMesh : public Mesh {
		
		public:
			AssimpMesh( const std::vector<MeshVertex> & vertices,const std::vector<unsigned int> & indices) {
				auto data = vertices.data();
				float * raw = (float*)data;
				std::shared_ptr<VertexBuffer> vbuff = std::make_shared<VertexBuffer>(raw, vertices.size()*sizeof(MeshVertex));
				VertexBufferLayout vbl;
				auto buff = std::make_pair(vbuff, vbl);
				m_BufferVector.push_back(buff);
				m_IBO = std::make_shared<unsigned int>(indices.data(),indices.size()*sizeof(unsigned int));
				count = indices.size();
			}
	};
}