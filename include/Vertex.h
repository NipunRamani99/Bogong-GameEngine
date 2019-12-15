#pragma once
#include <glm/glm.hpp>
template<typename T>
struct Vertex
{
	T x      = 0;
	T y      = 0;
	T z      = 0;
	float r  = 0.0f;
	float g  = 0.0f;
	float b  = 0.0f;
	float a  = 1.0f;
	float nx = 0.0f;
	float ny = 0.0f;
	float nz = 0.0f;
	float u  = 0.0f;
	float v  = 0.0f;
	char padding[16];
	Vertex<T>(){}
	Vertex<T>(glm::vec3 pos, glm::vec4 color, glm::vec3 normals=glm::vec3(0,0,0), glm::vec2 uv = glm::vec2(0,0))
	{
		x  = pos.x;
		y  = pos.y;
		z  = pos.z;
		r  = color.r;
		g  = color.g;
		b  = color.b;
		a  = color.a;
		nx = normals.x;
		ny = normals.y;
		nz = normals.z;
		u  = uv.x;
		v  = uv.y;
	}
};
template<typename T>
Vertex<T> CreateVertex(glm::vec3 p_Pos, glm::vec4 p_Color, glm::vec2 p_UV)
{
	Vertex<T> vertex;
	vertex.x = p_Pos.x;
	vertex.y = p_Pos.y;
	vertex.z = p_Pos.z;
	vertex.r = p_Color.r;
	vertex.g = p_Color.g;
	vertex.b = p_Color.b;
	vertex.a = p_Color.a;
	vertex.u = p_UV.x;
	vertex.v = p_UV.y;
	return vertex;
};
template<typename T>
Vertex<T> CreateVertex(glm::vec3 p_Pos, glm::vec4 p_Color, glm::vec2 p_UV, glm::vec3 p_Norm)
{
	Vertex<T> vertex;
	vertex.x  = p_Pos.x;
	vertex.y  = p_Pos.y;
	vertex.z  = p_Pos.z;
	vertex.r  = p_Color.r;
	vertex.g  = p_Color.g;
	vertex.b  = p_Color.b;
	vertex.a  = p_Color.a;
	vertex.u  = p_UV.x;
	vertex.v  = p_UV.y;
	vertex.nx = p_Norm.x;
	vertex.ny = p_Norm.y;
	vertex.nz = p_Norm.z;

	return vertex;
};