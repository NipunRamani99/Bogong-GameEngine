#pragma once
#include <string>
#include "Mesh.hpp"
#include "../VertexBufferLayout.hpp"
#include <unordered_map>


namespace bogong {
	/*
		class MeshData{
		   std::vector<Vertex<float>> m_Vertices;
		   std::vector<unsgined int> m_Indices;
		};	
	*/
	//Singleton this
	class MeshBank {
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