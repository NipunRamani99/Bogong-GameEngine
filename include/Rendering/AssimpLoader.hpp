#pragma once
#include<iostream>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include "Nodes/NodeBase.hpp"
#include "Nodes/ShapeNode.hpp"
#include "Mesh.hpp"
#include <optional>

namespace bogong {
	class AssimpFactory {
	
		
		std::shared_ptr<node::ShapeNode> ProcessNode(const aiScene * scene,aiNode * node) {
			//create mesh
			auto meshes = node->mMeshes;
			int n = node->mNumMeshes;
			std::vector<Mesh> meshvec;
			for (int i = 0; i < n; i++) {
				int idx = meshes[i];
				auto mesh =scene->mMeshes[idx];
				//auto meshptr = Mesh::CreateFromAssimpMesh(mesh);
			}
			//create a ShapeNode
			//std::shared_ptr<node::ShapeNode> mesh = std::make_shared<node::ShapeNode>(meshvec,root->Name);
			//then process and append child
			int nchild = node->mNumChildren;
			for (int i = 0; i < nchild; i++) {
				std::shared_ptr<node::ShapeNode>  nodeptr = ProcessNode(scene,node->mChildren[i]);
				//mesh->appendChild(node);
			}
			//return the node.
			//return mesh;
		}
		std::shared_ptr<node::ShapeNode> LoadModel(std::string path) {
			Assimp::Importer importer;
			
			const aiScene * scene = importer.ReadFile(path.c_str(), aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType);
			if (!scene) {
				std::cout << "error couldn't load scene from assimp.\n";
				std::system("pause");
				return nullptr;
			}
			auto ptr = ProcessNode(scene,scene->mRootNode);
			return ptr;
		}
	};
}