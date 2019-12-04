#include "Rendering/ShapeMesh.hpp"
#include "Rendering/Renderer.hpp"
class CameraPosVisualizer
{
	Vertex<float> vertex;
	std::shared_ptr<ShapeMesh> mesh;
	std::shared_ptr<Renderer> renderer;
	std::vector<Vertex<float>> vec;
	
public:
	CameraPosVisualizer()
	{
		initVertex();
		initMesh();
		initRenderer();
		vec.clear();
	}
	void initVertex()
	{
		
		vertex.r = 1.0f;
		vertex.g = 0.0f;
		vertex.b = 0.0f;
		vertex.a = 1.0f;
		vec.push_back(vertex);
	}
	void initMesh()
	{
		mesh = std::make_shared<ShapeMesh>(vec);
	}
	void initRenderer()
	{
		VertexBufferLayout layout;
		layout.AddElement<float>(3);
		layout.AddElement<float>(4);
		renderer = std::make_shared<Renderer>(layout);
		renderer->BindBuffers(mesh);
		renderer->SetDrawMode(GL_POINTS);
	
	}
	void SetShader(Shader & shader)
	{
		renderer->SetShader(shader);
	}
	void Draw()
	{
		glPointSize(10.0f);
		renderer->RenderMesh(mesh);
	}
};
