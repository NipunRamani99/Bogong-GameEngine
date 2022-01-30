/*
  (C) 2019 David Lettier
  lettier.com
*/

#version 420 core
out vec4 fragColor;
in vec2 TexCoords;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDepth;
uniform vec3 camPos;
uniform mat4 projection;
uniform mat4 view;
 uniform float maxDistance = 10.0;
 uniform float resolution  = 0.4;
 uniform int  steps = 20;
 uniform float thickness   = 0.008;
 uniform int iterationCount = 1024 ;
 float CalculateDepth(vec3 p) {
	vec4 clip_space = projection*view*vec4(p,1.0f);
	float clip_space_depth = clip_space.z / clip_space.w;
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;
	float depth = (((far-near)*(clip_space_depth))+near+far)/2.0f;
	return depth;
}
 float CalculateDepth2(vec3 p) {
	vec4 clip_space = projection*vec4(p,1.0f);
	float clip_space_depth = clip_space.z / clip_space.w;
	float far = gl_DepthRange.far;
	float near = gl_DepthRange.near;
	float depth = (((far-near)*(clip_space_depth))+near+far)/2.0f;
	return depth;
}
void main() {
  vec2 texSize  = textureSize(gPosition, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;
  vec4 uv = vec4(0.0);
  vec4 positionFrom     = view * texture(gPosition, texCoord);
  vec3 unitPositionFrom = normalize(positionFrom.xyz);
  vec3 normal           = normalize(texture(gNormal, texCoord).xyz);
  vec3 pivot            = normalize(reflect(unitPositionFrom, normal));
  vec4 startView =       vec4(positionFrom.xyz + (pivot *           0), 1);
  vec4 endView   =       vec4(positionFrom.xyz + (pivot * maxDistance), 1);
  vec4 startFrag      = startView;
  // Project to screen space.
  startFrag      = projection * startFrag;
  // Perform the perspective divide.
  startFrag.xyz /= startFrag.w;
  // Convert the screen-space XY coordinates to UV coordinates.
   startFrag.xy   = startFrag.xy * 0.5 + 0.5;
  // Convert the UV coordinates to fragment/pixel coordnates.
  startFrag.xy  *= texSize;
  // startFrag.xy -= TexCoords;
  
  vec4 endFrag      = endView;
       endFrag      = projection * endFrag;
       endFrag.xyz /= endFrag.w;
       endFrag.xy   = endFrag.xy * 0.5 + 0.5;
       endFrag.xy  *= texSize;
  vec2 frag  = startFrag.xy;
       uv.xy = frag / texSize;


  float deltaX    = endFrag.x - startFrag.x;
  float deltaY    = endFrag.y - startFrag.y;

  float useX      = abs(deltaX) >= abs(deltaY) ? 1 : 0;
  float delta     = mix(abs(deltaY), abs(deltaX), useX) * clamp(resolution, 0., 1.);
  vec2  increment = vec2(deltaX, deltaY) / max(delta, 0.001);
  float search0 = 0;
  float search1 = 0;
  int hit0 = 0;
  int hit1 = 0;
  float viewDistance = startView.z;
  float depth        = thickness;
  vec4 positionTo = positionFrom;
  positionTo = view * texture(gPosition, uv.xy);

  float i = 0.0f;
  
  for (i = 0; i < iterationCount ; ) {
    i+= 1.00f;
    frag      += increment;
    uv.xy      = frag / texSize;
    search1 =
      mix
        ( (frag.y - startFrag.y) / deltaY
        , (frag.x - startFrag.x) / deltaX
        , useX
        );
   positionTo = view * texture(gPosition, uv.xy);
   search1 = clamp(search1, 0.0, 1.0);
   viewDistance =   (startView.z * endView.z)/ mix(endView.z, startView.z, search1);    
   depth = (viewDistance - (positionTo.z))/positionTo.z; 

    if (depth > 0.0 && depth < thickness) {
      hit0 = 1;
      break;
    } else {
      search0 = search1;
    }
  }
     
  search1 = search0 + ((search1 - search0) / 2.0);
  
  i = 0.0f;
  for (; (i < steps); ) {
  i+=1.0f;    
  if(hit0 == 1) {
    frag       = mix(startFrag.xy, endFrag.xy, search1);
    uv.xy      = frag / texSize;
    positionTo = view * texture(gPosition, uv.xy);

   // viewDistance = (CalculateDepth(startView.xyz)*CalculateDepth(endView.xyz)) / mix(CalculateDepth(endView.xyz), CalculateDepth(startView.xyz), search1);
   viewDistance =   (startView.z * endView.z)/ mix(endView.z, startView.z, search1);
   depth        = (viewDistance - positionTo.z);  //(positionTo.z) - (texture(gDepth, uv.xy).x);

    if (depth > 0.0 && depth < thickness) {
       hit1 = 1;      
       search1 = search0 + ((search1 - search0) / 2);
      
    } else {
      float temp = search1;
      search1 = search1 + ((search1 - search0) / 2);
      search0 = temp;
    }
      }
  }
  
  float visibility =
      hit1 * positionTo.w
    * ( 1
      - max
         ( dot(-unitPositionFrom, pivot)
         , 0
         )
      )
    * ( 1
      - clamp
          ( depth / thickness
          , 0
          , 1
          )
      )
    * ( 1
      - clamp
          (   length(positionTo - positionFrom)
            / maxDistance
          , 0
          , 1
          )
      )
    * (uv.x < 0 || uv.x > 1 ? 0 : 1)
    * (uv.y < 0 || uv.y > 1 ? 0 : 1);
    
  visibility = clamp(visibility, 0, 1);

  uv.ba = vec2(visibility);

  fragColor = uv;
}