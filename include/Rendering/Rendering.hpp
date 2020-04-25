#pragma once
#include <iostream>
#include "Nodes/ShapeNode.hpp"
#include <queue>
namespace bogong {

	struct RenderQueueItem {
		std::shared_ptr<node::ShapeNode> node;
		int priority = 1;
	};
	class TexturedPassDude {

	public:
	private:
		//Texture shader

	};
	class RendererDude {
		
	private:
		/*
			std::vector<RenderQueueItem> render_dudes;
			
		*/
		//has a queue of renderable dudes with what to render using
		//has a list of lights
		//keep count of # of lights per type
	public:
		/*
		void AddRenderDudes(RenderDude & renderdude){
			//add renderdude to queue
			
		}
		void AddLight(Light & light){
			//Add light sources	
		}
		void InitiateRender(){
			//iterate through render dudes and then draw shit
			//
		}
		
		*/
	};
}