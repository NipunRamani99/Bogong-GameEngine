#include "Renderer.hpp"

 bogong::Renderer::Renderer()
{
	m_VAO = VertexArray();
}

 void bogong::Renderer::SetShader(Shader p_Shader)
{
	m_Shader = p_Shader;
}

 void bogong::Renderer::SetDrawMode(GLenum p_DrawMode)
{
	m_DrawMode = p_DrawMode;
}

 void bogong::Renderer::SetDrawCall(DrawCall p_DrawCall)
{
	m_DrawCall = p_DrawCall;
}
