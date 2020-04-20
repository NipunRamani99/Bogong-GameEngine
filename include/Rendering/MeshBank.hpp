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
		
	public:
		static std::shared_ptr<Mesh> CreateFromAssimpMesh(aiMesh * mesh)
		{
			auto v = mesh->mVertices;
			auto uv = mesh->mTextureCoords;
			
			auto normals = mesh->mNormals;
			std::vector<MeshVertex> vertices;
			vertices.resize(mesh->mNumVertices);
			for (int i = 0; i < mesh->mNumVertices; i++) {
				vertices[i].x = v[i].x;
				vertices[i].y = v[i].y;
				vertices[i].z = v[i].z;

				if (normals) {
					vertices[i].nx = normals[i].x;
					vertices[i].ny = normals[i].y;
					vertices[i].nz = normals[i].z;
				}
				else
				{
					vertices[i].nx = 0.0f;
					vertices[i].ny = 0.0f;
					vertices[i].nz = 0.0f;
				}
			
				vertices[i].u = uv[0][i].x;
				vertices[i].v = uv[0][i].y;
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