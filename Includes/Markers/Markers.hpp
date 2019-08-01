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
struct MarkerParameters
{
	glm::vec3 m_Pos = { 0,0,0 };
	glm::vec4 m_FG_color = { 1,0,0,1 };
	glm::vec4 m_BG_color = { 0,1,0,1 };
	float m_Size = 0.005f;
	float m_Antialias = 1.0;
	float m_Linewidth = 0.001;
};
class Marker
{
	MarkerParameters m_Params;
	ShapeMesh m_Mesh;
	Renderer m_Renderer;
	VertexBufferLayout vbl;
public:
	Marker()
	{
	}
	Marker(glm::vec3 pos, glm::vec4 fg_color, glm::vec4 bg_color, float size = 0.5f, float antialias = 1.0, float linewidth = 0.01)
	{
		m_Params.m_Pos       = pos;
		m_Params.m_FG_color  = fg_color;
		m_Params.m_BG_color  = bg_color;
		m_Params.m_Size      = size;
		m_Params.m_Antialias = antialias;
		m_Params.m_Linewidth = linewidth;
		std::vector<Vertex<float>> vertex(1);
		vertex[0]   = Vertex<float>(pos, fg_color, { 0,0,0 }, { 0,0 });
		vbl.AddElement<float>(3);
		vbl.AddElement<float>(4);
		m_Mesh      = std::move(ShapeMesh(vertex));
		m_Renderer  = std::move(Renderer(m_Mesh, vbl));
		m_Renderer.SetDrawMode(GL_POINTS);
	}
	void SetShader(Shader  m_Shader)
	{
		m_Renderer.SetShader(m_Shader);
	}
	void Draw()
	{
		m_Renderer.RenderMesh();
	}
};