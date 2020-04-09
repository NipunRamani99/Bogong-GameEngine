#include "Scene.hpp"
#include <vector>
#include "Nodes/ShapeNode.hpp"
bogong::Scene::Scene() {}

 void bogong::Scene::SetRootNode(std::shared_ptr<node::NodeBase> node) {
	root_node = node;
}

 /*
   
   //Singleton this
   class MeshDataCache{
       public:
	   
	   static std::unordered_map<std::string,ptrBuffer> MeshDataMap;
	   static ptrBuffer getBuffer(std::string name);
	   static void AddBuffer(std::string name, ptrBuffer buffer)
	   {
	      MeshDataMap[name] = buffer;
	   }
   }



*/

 /*



 class Renderer{
	private:
	   Shader sh;
	   VAO vao;
	   std::vector<ptrBuffer> shapes;
    public:
	void initial_traversal( Node * node){
		//Bind vao
	    //Buffer data
		for( buffer in shapes ) bindBuffer(buffer);
	}
	void RenderMesh( Meshptr mesh ){
	   //bind vao
		vao->Bind();
	   //bind shader
		sh->Bind();
	   //bind buffers
	    mesh->BindData();
	   //call glDrawCall
	    glDrawArrays(,,); 
	}
	void SetLightData(PointLight pl){
	   sh.setInt("enablePointLight",1);
	   //set uniforms
	   sh.setVec3("pointlight.ambient",pl.ambient);
	   sh.setVec3("pointlight.diffuse",pl.diffuse);
	   sh.setVec3("pointlight.specular",pl.specular);
	   sh.setVec3("pointlight.pos",pl.pos);
	   sh.setFloat("pointlight.constant",pl.constant);
	   sh.setFloat("pointlight.constant",pl.linear);
	   sh.setFloat("pointlight.constant",pl.quadratic);
	}
	void SetLightData(SpotLight spl){
	   sh.setInt("enableSpotLight",1);
	   //set uniforms
	   sh.setVec3("spotlight.ambient",spl.ambient);
	   sh.setVec3("spotlight.diffuse",spl.diffuse);
	   sh.setVec3("spotlight.specular",spl.specular);
	   sh.setVec3("spotlight.pos",spl.pos);
	   sh.setFloat("spotlight.innercutoff",spl.innercutoff);
	   sh.setFloat("spotlight.outercutoff",spl.outercutoff);
	}
	void SetLightData(DirectionalLight dl){
	   sh.setInt("enable
	   sh.setVec3("directionallight.ambient",spl.ambient);
	   sh.setVec3("directionallight.diffuse",spl.diffuse);
	   sh.setVec3("directionallight.specular",spl.specular);
	   sh.setVec3("directionallight.direction",spl.dir);
	  

	}
	void SetMaterial(MaterialData mtl){
	   sh.setVec3("material.ambient"   ,mtl.ambient);
	   sh.setVec3("material.diffuse"   ,mtl.diffuse);
	   sh.setVec3("material.specular"  ,mtl.specular);
	   sh.setFloat("material.shininess",mtl.shininess);
	 }

 }
 */
 /*
	class RenderGPUStateCache{
	   private:
	     glm::mat4 model;
		 glm::mat4 view;
	
	   public:
	     getModel();
		 getView();
		 applyTransform();

	}
 
 
 */

 //         
 //         
 //         R              R->S1->S3
 //       /   \            R->S1->S4
 //      S1   S2           R->S1
 //      /\   /\           R->S2->S5
 //    S3 S4 S5 S6         R->S2->S6

//

