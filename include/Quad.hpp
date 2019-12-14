#include "Rendering/Renderer.hpp"
#include "Rendering/ShapeMesh.hpp"
#include <memory>
#include <vector>
namespace bogong {
	class QuadMesh : public ShapeMesh
	{
		std::vector<Vertex<float>> vertices;
		
	public:
		QuadMesh();
	};
	class Quad {
		std::shared_ptr<QuadMesh> mesh;
		std::shared_ptr<Renderer> renderer;
		Quad() {
			mesh = std::make_shared<QuadMesh>();
		}
	};
}