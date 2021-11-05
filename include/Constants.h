#pragma once
#include <string>
namespace bogong {
    const static std::string SHADER_PATH = "./Shaders";
    const static std::string INFINITE_PLANE_VERTEX_SHADER =
        SHADER_PATH + "/ScreenVertexShader.glsl";
    const static std::string INFINITE_PLANE_RENDER_PASS_FRAGMENT_SHADER =
        SHADER_PATH + "/ScreenFragmentRenderPassShader.glsl";
    const static std::string INFINITE_PLANE_DEPTH_PASS_FRAGMENT_SHADER =
        SHADER_PATH + "/ScreenFragmentShaderDepthPass.glsl";
    const static std::string INFINITE_PLANE_DEPTH_PASS_GEOMETRIC_SHADER =
        SHADER_PATH + "/ScreenGeometricShaderDepthPass.glsl";
    
    const static float vertices[] = {
               1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f
    };
    const static float screen_vertices[] = {
            1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,
            1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f
    };
}
