#include "Scene.hpp"

 bogong::Scene::Scene() {}

 void bogong::Scene::SetRootNode(std::shared_ptr<node::NodeBase> node) {
	root_node = node;
}


 void bogong::Scene::Draw(Shader & shader) {
	 std::stack<std::shared_ptr<node::NodeBase>> st;
	 st.push(root_node);
	 while (!st.empty())
	 {
		 auto node = st.top();

		 auto type = node->GetType();
		 switch (type)
		 {
		 case node::NodeType::Group:
		 {
			 auto vn = node->GetChilds();
			 if (vn.size() != 0) {
				 for (int i = vn.size() - 1; i >= 0; i--) {
					 st.push(vn[i]);
				 }
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
			 //Bind mesh data
			 /*
			     std::shared_ptr<ShapeNode> shnode = std::dynamic_pointer_cast<ShapeNode>(node);
			     renderer->bindData( );
			 
			 */
			 //Make draw call
			 break;
		 }
		 default:
		 {
			 std::cout << "Unknown node type.\n";
			 std::system("pause");
			 break;
		 }
		 }
	 }
 }


