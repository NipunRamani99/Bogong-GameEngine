#include "../Rendering/ShapeMesh.hpp"
#include "../Rendering/Renderer.hpp"

/*Disc Marker Attributes
1.)mat4 ortho -> 
2.)float size
3.)float orientation
4.)float linewidth
5.)float antialias
6.)vec3 pos
7.)vec4 fg_color and bg_color;
*/
class Marker
{
	glm::vec3 m_Pos = {0,0,0};
	glm::vec4 m_FG_color = { 1,0,0,1 };
	glm::vec4 m_BG_color = { 0,1,0,1 };
	float m_Size = 0.005f;
	float m_Antialias = 1.0;
	float m_Linewidth = 0.001;
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
public:
	Marker()
	{

	}
	Marker(glm::vec3 pos, glm::vec4 fg_color, glm::vec4 bg_color, float size = 0.5f, float antialias = 1.0, float linewidth = 0.01)
	{
		m_Pos = pos;
		m_FG_color = fg_color;
		m_BG_color = bg_color;
		m_Size = size;
		m_Antialias = antialias;
		m_Linewidth = linewidth;
	}
};