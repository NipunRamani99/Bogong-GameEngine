#version 330 core
in vec3 fragPos;
in vec4 fragColor;
out vec4 color;
void main()
{  
   float x = fragPos.x*10.0;
   float z = fragPos.z*10.0;
   bool k = (pow(x,2)+pow(z,2)<10.0);
   bool t = abs(x)==abs(z);
   if(k)
   {
		
	   color = vec4(1,0,1,1);
   }
   else{
      color = vec4(0,0,0,1);
   }
}	