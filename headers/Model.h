#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
protected:
	std::vector<Mesh> m_Meshes;
public:

	Model()
		:
		m_Meshes()
	{

	}
	Model(const std::string p_Path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(p_Path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		if (!scene)
		{
			std::cout << (importer.GetErrorString());

		}
		aiMesh ** mesh = (scene->mMeshes);
		int num = scene->mNumMeshes;
		for (int i = 0; i < num; i++)
		{

			std::cout << "Mesh #" << i + 1 << ":" << "\n";
			DisplayMeshDetails(*mesh[i]);
			std::cout << "Mesh Name: ";
			m_Meshes.push_back(std::move(*mesh[i]));
			m_Meshes[i].SetTexture("assets/models/textures");
			std::cout << "\n";
		}
		for (auto & v : m_Meshes)
		{
			v.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}

	}
	void Draw()
	{
		for (auto&v : m_Meshes)
		{
			v.Draw();
		}
	}
	void _Draw()
	{
		for (auto & v : m_Meshes)
		{
			v._Draw();
		}
	}
	void DisplayMeshDetails(const aiMesh & mesh)
	{
		std::cout << "1. Has bones: " << mesh.HasBones() << "\n";
		std::cout << "2. Has faces: " << mesh.HasFaces() << "\n";
		std::cout << "3. Has normals: " << mesh.HasNormals() << "\n";
		std::cout << "4. Has positions: " << mesh.HasPositions() << "\n";
		std::cout << "5. Has Tangents and Bitangents: " << mesh.HasTangentsAndBitangents() << "\n";
		std::cout << "6. Has Texture coordinates: " << mesh.HasTextureCoords(0) << "\n";
		std::cout << "8. # of Animated Meshes " << mesh.mNumAnimMeshes << "\n";
		std::cout << "9. # of Bones " << mesh.mNumBones << "\n";
		std::cout << "10. # of Faces " << mesh.mNumFaces << "\n";
		std::cout << "11. # of UV Components ";
		for (int i = 0; i < 8; i++)
		{
			std::cout << i + 1 << "." << mesh.mNumUVComponents[i] << " ";
		}
		std::cout << "\n";
		std::cout << "12. # of Vertices " << mesh.mNumVertices << "\n";

	}
	inline void SetScale(const glm::vec3 p_Scale)
	{
		for (auto & m : m_Meshes)
		{
			m.SetScale(p_Scale);
		}
	}
	void SetShader(const Shader p_Shader)
	{
		for (auto & m : m_Meshes)
		{
			m.SetShader(p_Shader);
		}
	}
	Model & operator=(Model && p_Object)
	{
		m_Meshes = std::move(p_Object.m_Meshes);
		return *this;
	}
};