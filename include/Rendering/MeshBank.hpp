#pragma once
#include <string>
#include "Mesh.hpp"
#include "../VertexBufferLayout.hpp"
#include <unordered_map>
#include "assimp/scene.h"
#include "AssimpMesh.hpp"
namespace bogong {

	//Singleton this
	class MeshBank {
		
		static std::shared_ptr<Mesh> CreateFromAssimpMesh(aiMesh * mesh)
		{
			auto v = mesh->mVertices;
			auto uv = mesh->mTextureCoords;
			auto normals = mesh->mNormals;
			std::vector<MeshVertex> vertices;
			vertices.resize(mesh->mNumVertices);
			for (int i = 0; i < mesh->mNumVertices; i++) {
				vertices[i].pos = glm::vec3(v[i].x, v[i].y, v[i].z);
				vertices[i].norms = glm::vec3(normals[i].x, normals[i].y, normals[i].z);
				if(mesh->mTextureCoords[0])
				vertices[i].uv = glm::vec2(uv[0][i].x, uv[0][1].y);
				else
				vertices[i].uv = glm::vec2(0.0f, 0.0f);
			}
			int nfaces = mesh->mNumFaces;
			std::vector<unsigned int> indices;
			for (size_t i = 0; i < nfaces; i++) {
				auto face = mesh->mFaces[i];
				for (size_t j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}
			std::shared_ptr<AssimpMesh> m = std::make_shared<AssimpMesh>(vertices,indices);
			return std::dynamic_pointer_cast<Mesh>(m);
		}
		/*
		using ptrMesh = std::shared_ptr<MeshData>;
		private:
			std::unordered_map<std::string,ptrMesh>
		public:
			MeshBank();
			ptrMesh getMesh( std::string name);
			void setMesh(std::string name, ptrMesh);
		*/
	};
}