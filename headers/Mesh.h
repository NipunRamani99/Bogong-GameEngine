#pragma once
#include<vector>
#include<glm/gtx/transform.hpp>
#include "Vertex.h"
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "Texture.h"
#include "IndexBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ICallbacks.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Mesh
{
protected:
	std::vector<Vertex<float>>  m_Vertices;
	std::vector<unsigned int>   m_Indices;
	glm::vec3 m_Position=glm::vec3(0.0f,0.0f,0.0f);
	glm::mat4 m_Model = glm::mat4(1.0f);
	std::string id = "";
	std::string name = "";
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;
	Texture m_Tex;
	Shader m_Shader;
	bool m_IsConstructed = false;
	GLenum m_Mode = GL_TRIANGLES;
public:
	Mesh()
	{
		m_IsConstructed = false;
	}
	Mesh(Mesh && p_Model)
	{
		p_Model.m_IsConstructed = false;
		m_VBO      = std::move(p_Model.m_VBO);
		m_IBO      = std::move( p_Model.m_IBO);
		m_VAO      = std::move(p_Model.m_VAO);
		m_Shader   = std::move(m_Shader);
		m_Vertices = std::move(p_Model.m_Vertices);
		m_Indices  = std::move(p_Model.m_Indices);
		m_Position = p_Model.m_Position;
		m_Model    = p_Model.m_Model;
		id         = p_Model.id;
		name       = std::move(p_Model.name);
		m_Tex      = std::move(p_Model.m_Tex);
		m_Mode = std::move(p_Model.m_Mode);
		m_IsConstructed = true;
	}
	Mesh(std::vector<Vertex<float>> & p_Vertices, std::vector<unsigned int> & p_Indices)
	{
		m_Vertices = p_Vertices;
		m_Indices  = p_Indices;
		id         = GenerateRandomString(10);
		m_IsConstructed = true;
	}
	Mesh(glm::vec3 p_Position)
	{
		m_Position = p_Position;
		m_Model    = glm::translate(m_Model, m_Position);
		id         = GenerateRandomString(10);
		m_IsConstructed = true;
	}
	Mesh(aiMesh & mesh)
	{
		std::cout << mesh.mName.C_Str();
		m_IsConstructed = true;
		
		id   = GenerateRandomString(10);
		name = mesh.mName.C_Str();
		
		m_Vertices.resize(mesh.mNumVertices);
		m_Indices.resize(mesh.mNumFaces*mesh.mFaces->mNumIndices);
		int i = 0;
		for(auto & v : m_Vertices)
		{
			v.x = mesh.mVertices[i].x;
			v.y = mesh.mVertices[i].y;
			v.z = mesh.mVertices[i].z;
			v.u = mesh.mTextureCoords[0][i].x;
			v.v = mesh.mTextureCoords[0][i].y;
			i++;
		}
		int numIndices = mesh.mFaces->mNumIndices;
		for (i = 0; i < mesh.mNumFaces; i++)
		{
			for (int j = 0; j < numIndices; j++)
			{
				m_Indices[i * numIndices + j] = mesh.mFaces[i].mIndices[j];
			}
		}
		m_VBO = VertexBuffer(m_Vertices.data(),m_Vertices.size()*sizeof(Vertex<float>));
		m_IBO = IndexBuffer(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));
	}
	Mesh & operator=(Mesh && p_Mesh)
	{
		m_IBO      = std::move(p_Mesh.m_IBO);
		m_VAO      = std::move(p_Mesh.m_VAO);
		m_VBO      = std::move(p_Mesh.m_VBO);
		m_Indices  = std::move(p_Mesh.m_Indices);
		m_Vertices = std::move(p_Mesh.m_Vertices);
		m_Position = std::move(p_Mesh.m_Position);
		m_Mode     = std::move(p_Mesh.m_Mode);
		m_Model    = std::move(p_Mesh.m_Model);
		m_Tex      = std::move(p_Mesh.m_Tex);
		name       = std::move(p_Mesh.name);
		id         = std::move(p_Mesh.id);
		m_Shader = std::move(p_Mesh.m_Shader);
		p_Mesh.m_IsConstructed = false;
		m_IsConstructed = true;
		return *this;
	}
    virtual ~Mesh()
	{
		if (m_IsConstructed)
			std::cout << "Model ID: " << id << " has been destroyed.";
	}
	void SetTexture(std::string p_Path)
	{
		p_Path = p_Path + "/" + name + ".png";
        int width          = 0;
		int height         = 0;
		int channels       = 0;
	    unsigned char * imgData = stbi_load(p_Path.c_str(),&width,&height,&channels,0);
		m_Tex = Texture(width, height,imgData);
		
	}
	void SetVertices(std::vector<Vertex<float>> & p_Vertices)
	{
		m_Vertices = p_Vertices;
		m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex<float>));

	}
	void SetVertices(float vertices[])
	{
		
		m_VBO = VertexBuffer(vertices,sizeof(float)*360);

	}
	void SetIndices(std::vector<unsigned int> & p_Indices)
	{
		m_Indices = p_Indices;
		m_IBO = IndexBuffer(m_Indices.data(), m_Indices.size() * sizeof(unsigned int));
	}
	void SetShader(Shader p_Shader)
	{
		m_Shader = p_Shader;
	}
	GLuint GetShaderId()
	{
		return m_Shader.GetProgramID();
	}
	void SetDrawMode(GLenum p_DrawMode)
	{
		m_Mode = p_DrawMode;
	}
	void SetPosition(glm::vec3 p_Position)
	{
		m_Position = p_Position;
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, p_Position);
	}
	
	void SetScale(glm::vec3 p_Scale)
	{
		const auto translation = glm::translate(glm::mat4(1), glm::vec3(1.0f,0.0f,0.0f));
		const auto rotation = glm::rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3(1.0f,0.0f,0.0f));
		const auto scale = glm::mat4(1);

		const auto model_matrix_ = scale * rotation * translation * glm::mat4(1);
		
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, m_Position);
		m_Model = glm::scale(m_Model, p_Scale);
	}
	void Bind()
	{
		m_VAO.Bind();
		m_VBO.Bind();
		m_IBO.Bind();
		m_Shader.Bind();
		if (m_Tex.m_TexID != 0)
		{
			m_Shader.setBool("isTextured", true);
			m_Tex.Bind();
			glEnableVertexArrayAttrib(m_VAO.GetID(), 2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), (void*)(sizeof(float) * 7));
		}
		glEnableVertexArrayAttrib(m_VAO.GetID(), 0);
		glEnableVertexArrayAttrib(m_VAO.GetID(), 1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex<float>), (void*)0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex<float>), (void*)(sizeof(float) * 3));
	}
	
	void Unbind()
	{
		glDisableVertexArrayAttrib(m_VAO.GetID(), 0);
		glDisableVertexArrayAttrib(m_VAO.GetID(), 1);
		error(); 
		glDisableVertexArrayAttrib(m_VAO.GetID(), 2);
		error();
		m_Shader.setBool("isTextured", false);
		error();
		if(m_Tex.m_TexID != 0)
		m_Tex.Unbind();
		error();
		m_VBO.Unbind();
		error();
		m_IBO.Unbind();
		m_VAO.Unbind();
		error();
	}
	void Draw()
	{
		Bind();
		error();
		m_Shader.setMat4("model", m_Model);
		error();
		glDrawElements(m_Mode, m_Indices.size(), GL_UNSIGNED_INT, 0);
		error();
		Unbind();
		error();
	}
	void _Draw()
	{
		m_VAO.Bind();
		m_VBO.Bind();
		m_Shader.Bind();
		glEnableVertexArrayAttrib(m_VAO.GetID(), 0);
		glEnableVertexArrayAttrib(m_VAO.GetID(), 1);
		glEnableVertexArrayAttrib(m_VAO.GetID(), 3);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE,  10*sizeof(float), (void*)(sizeof(float) * 0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)(sizeof(float) * 3));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)(sizeof(float) * 7));
		m_Shader.setMat4("model", m_Model);
		glDrawArrays(GL_TRIANGLES, 0, m_VBO.GetSize() / (sizeof(float) * 10));
		glDisableVertexArrayAttrib(m_VAO.GetID(), 0);
		glDisableVertexArrayAttrib(m_VAO.GetID(), 1);
		glDisableVertexArrayAttrib(m_VAO.GetID(), 3);
		m_VBO.Unbind();
		m_VAO.Unbind();
	}
private:
	std::string GenerateRandomString(int p_NumChars)
	{
		std::string str="";
		unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
		std::default_random_engine rd(seed);
		std::uniform_int_distribution<int> intDist(0,25);
		for (int i = 0; i < p_NumChars;i++)
		{
			str += 'A' + (char)intDist(rd);
		}
		return str;
		
	}
	
};