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
 
 void bogong::Scene::Draw(Shader & shader) {
	 std::vector<std::shared_ptr<node::NodeBase>> st;
	 st.push_back(root_node);
	 auto it = st.begin();
	 auto end = st.end();
	 while (it!=end)
	 {
		 auto node = *it;

		 auto type = node->GetType();
		 switch (type)
		 {
		 case node::NodeType::Group:
		 {
			 auto vn = node->GetChilds();
			 for (auto n : vn) {
				 st.push_back(n);
			 }
			 break;
		 }
		 case node::NodeType::Light:
		 {
			 //Get type of light
			 /*
				std::shared_ptr<LightNodeBase> light = std::dynamic_pointer_cast<LightNodeBase>(node);
				auto type = light.GetType();
				switch( type )
				{
				//Process them
					case Point:
					{
						//Set Shader Data.
						auto pl = light->getData();
						renderer->SetLight( pl );
						break;
					}
					case Spotlight:
					{
						//Set Shader Data.

						break;
					}
					case Directional:
					{
					    //Set Shader Data.
						break;
					}
					default:
					{
						std::cout<<"Invalid light type.\n";
						break;
					}
				}
			 */
			 //Set the parameters
			 break;
		 }
		 case node::NodeType::Material:
		 {
			 //Set the parameters
			 /*
			      std::shared_ptr<Material> material= std::dynamic_pointer_cast<Material>(node);
				  auto mtl = material.GetMaterial();
				  shader.setVec3( "material.ambient", mtl.ambient );
				  shader.setVec3( "material.diffuse", mtl.diffuse);
				  shader.setVec3( "material.specular", mtl.specular );
			 */
			 break;
		 }
		 case node::NodeType::Shape:
		 {

			 auto shape_node = std::dynamic_pointer_cast<node::ShapeNode>(*it);
			 
			 break;
		 }
		 default:
		 {
			 std::cout << "Unknown node type.\n";
			 std::system("pause");
			 break;
		 }
		   it++;
		 }
	 }
 }


