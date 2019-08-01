#version 330 core

// Ouput data
in vec4 fragColor;
out vec4 color;
void main()
{
   if(abs(gl_FragCoord.x) == abs(gl_FragCoord.z) )
	color = vec4(0,1,0,1);
   else
    color = vec4(1,0,0,1);
}	