#pragma once
#include<iostream>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include "Nodes/NodeBase.hpp"
#include "Nodes/ShapeNode.hpp"
#include "MeshBank.hpp"
#include "../Material.hpp"
#include "../Models/TextureFactory.hpp"
#include <optional>
#include <Shlwapi.h>
namespace bogong {
	
	class AssimpFactory {
	
	public:
		static bool isPathFileSpec(std::string dir) {
			if (dir == "")return false;
			BOOL b = PathIsFileSpecA(dir.c_str());
			
			return b == FALSE?false:true;
		}
		static bool isPathValid(std::string path) {
			if (path == "") return false;
			BOOL b = PathFileExistsA(path.c_str());
			return b == FALSE ? false : true;
		}
		static std::shared_ptr<node::ShapeNode> ProcessNode(const aiScene * scene,aiNode * node,std::string  path) {
			//create mesh
			auto meshes = node->mMeshes;
			int n = node->mNumMeshes;
			aiMatrix4x4 transform;
			std::vector<std::shared_ptr<Mesh>> meshvec;
			for (int i = 0; i < n; i++) {
				int idx = meshes[i];
				auto mesh =scene->mMeshes[idx];
				auto meshptr = MeshBank::CreateFromAssimpMesh(mesh);
				auto matidx = mesh->mMaterialIndex;
				
				if (matidx>=0) {
					aiMaterial * mat = scene->mMaterials[matidx];
					std::string name;
					aiString str;
					std::shared_ptr<MaterialData<true>> mtl = std::make_shared<MaterialData<true>>();
					mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
					name = str.C_Str();
					if (isPathFileSpec(name))
						name = path + "\\" + str.C_Str();

					if (isPathValid(name)) {
						
						mtl->diffuse = TextureManager::make_texture			(name);

					}
					
					str.Clear();
					mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
					
					name = str.C_Str();
					if (isPathFileSpec(name))
						name = path + "\\" + str.C_Str();

					if (isPathValid(name)) {
						mtl->specular = TextureManager::make_texture(name);
					}
					mtl->shininess = 16.0f;
					meshptr->setMaterialTex(mtl);
				}
				else 
				{
					std::shared_ptr<MaterialData<false>> mtl = std::make_shared<MaterialData<false>>();
					mtl->ambient                             = glm::vec3(0.1f, 0.1f, 0.1f);
					mtl->diffuse                             = glm::vec3(0.2f,0.2f,0.2f);
					mtl->specular                            = glm::vec3(0.3f, 0.2f, 0.5f);
					mtl->shininess                           = 16.0f;
					meshptr->setMaterialColoured(mtl);
				}

				//mesh->setMaterial(matdata);

				meshvec.push_back(meshptr);
			}
			//create a ShapeNode
			auto transformation = node->mTransformation;
			auto name = node->mName;
			aiVector3D pos,scale,rotation;	 
			glm::vec3 p, s, r;
			transformation.Decompose(scale, rotation, pos);
			p.x = pos.x;
			p.y = pos.y;
			p.z = pos.z;
			s.x = scale.x;
			s.y = scale.y;
			s.z = scale.z;
			r.x = rotation.x;
			r.y = rotation.y;
			r.z = rotation.z;
			
			std::shared_ptr<node::ShapeNode> mesh = std::make_shared<node::ShapeNode>(meshvec,glm::vec3(1.0f,0.0f,0.0f),p,name.C_Str());
			mesh->Rotate(r);
			mesh->Scale(s);
			mesh->setPos(p);
			mesh->UpdateModel();
			//then process and append child
			int nchild = node->mNumChildren;
			for (int i = 0; i < nchild; i++) {
				std::shared_ptr<node::ShapeNode>  nodeptr = ProcessNode(scene,node->mChildren[i],path);
				mesh->AddChild(nodeptr);
			}
			//return the node.
			return mesh;
		}
		static std::string GetDirectory(std::string path) {
			size_t l = path.rfind('\\');
			std::string dir = "";
			dir.resize(l);
			std::copy(path.begin(), path.begin()+l, dir.begin());

			return dir;
		}
		static std::shared_ptr<node::ShapeNode> LoadModel(std::string path) {
			Assimp::Importer importer;
			
			const aiScene * scene = importer.ReadFile(path.c_str(), 
				aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);
			
			if (!scene) {
				std::cout << "error couldn't load scene from assimp.\n";
				std::system("pause");
				return nullptr;
			}
			
			std::string dir = GetDirectory(path);
			auto ptr = ProcessNode(scene,scene->mRootNode,dir);
			return ptr;
		}
	};
}