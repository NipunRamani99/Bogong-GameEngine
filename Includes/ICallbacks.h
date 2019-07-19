#pragma once
#include "Callbacks.h"
namespace ICallbacks
{
	void SetShader(Shader p_Shader)
	{
		glUseProgram(p_Shader.GetProgramID());
		Callbacks::cam->setShader(p_Shader);
		Callbacks::freecam->SetShader(p_Shader);
		Callbacks::updateCamera();
	}
	void AddShader(Shader p_Shader)
	{
		Callbacks::cam->AddShader(p_Shader);
		Callbacks::freecam->AddShader(p_Shader);
	}
};
