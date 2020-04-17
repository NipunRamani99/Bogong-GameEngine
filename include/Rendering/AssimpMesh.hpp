#pragma once
#include "Mesh.hpp"
namespace bogong {
	struct MeshVertex {
		float x=0, y=0, z=0, nx=0, ny=0, nz=0,u=0,v = 0;
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
				unsigned int * d = (unsigned int *)indices.data();
				m_IBO = std::make_shared<IndexBuffer>(d,indices.size()*sizeof(unsigned int));
				
				count = indices.size();
			}
	};
}