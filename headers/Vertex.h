#pragma once

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
};
template<typename T>
Vertex<T> CreateVertex(glm::vec3 p_Pos, glm::vec4 p_Color)
{
	Vertex<T> vertex;
	vertex.x = p_Pos.x;
	vertex.y = p_Pos.y;
	vertex.z = p_Pos.z;
	vertex.r = p_Color.r;
	vertex.g = p_Color.g;
	vertex.b = p_Color.b;
	vertex.a = p_Color.a;
	return vertex;
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