#pragma once
namespace bogong {
const static float cubeVertices[108] = {
            -0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

             0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,

             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,

             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,


            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,

            -0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f,

        };
const static  float quadVertices[30] = {
            // positions     // colors
            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
            -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,

            -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
             0.05f,  0.05f,  0.0f, 1.0f, 1.0f,
             0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
};        
const static  float quadScreenVertices[24] = {   
            // vertex attributes for a quad that fills the entire screen 
            //in Normalized Device Coordinates.
            // positions   // texCoords
             -1.0f,  1.0f,  0.0f, 1.0f,
             -1.0f, -1.0f,  0.0f, 0.0f,
              1.0f, -1.0f,  1.0f, 0.0f,
             
             -1.0f,  1.0f,  0.0f, 1.0f,
              1.0f, -1.0f,  1.0f, 0.0f,
              1.0f,  1.0f,  1.0f, 1.0f
        };
const static float planeVertices[48] = {
            // positions            // normals         // texcoords
             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
            -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
             10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
             10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        };
const static float texturedCube[288] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
const static unsigned int SCREEN_WIDTH = 1280;
const static unsigned int SCREEN_HEIGHT = 740;
const static unsigned int SHADOW_WIDTH = 1280;
const static unsigned int SHADOW_HEIGHT = 740;
const static std::string WOOD_TEXTURE = "./assets/models/nicewood.png";
const static std::string SHADER_DIRECTORY_PATH = "./Shaders/learnopengl";
const static std::string ADVANCED_LIGHTNING_VERTEX_SHADER = SHADER_DIRECTORY_PATH 
+ "/advanced_lighting.vs";
const static std::string ADVANCED_LIGHTNING_FRAGMENT_SHADER = SHADER_DIRECTORY_PATH +
"/advanced_lighting.fs";
const static std::string DEBUG_QUAD_VERTEX_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/debug_quad.vs";
const static std::string DEBUG_QUAD_FRAGMENT_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/debug_quad.fs";
const static std::string SIMPLE_DEPTH_VERTEX_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/simple_depth_shader.vs";
const static std::string SIMPLE_DEPTH_FRAGMENT_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/simple_depth_shader.fs";
const static std::string SHADOW_PASS_VERTEX_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/shadow_pass.vs";
const static std::string SHADOW_PASS_FRAGMENT_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/shadow_pass.fs";
const static std::string SIMPLE_VERTEX_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/simple_shader.vs";
const static std::string SIMPLE_FRAGMENT_SHADER = SHADER_DIRECTORY_PATH
+ "/shadowmapping/simple_shader.fs";
}