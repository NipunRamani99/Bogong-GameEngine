/*
  (C) 2019 David Lettier
  lettier.com
*/

#version 420 core

uniform sampler2D ssrReflectedUV;
uniform sampler2D gAlbedoSpec;

out vec4 fragColor;

void main() {
  int   size       = 6;
  float separation = 2.0;

  vec2 texSize  = textureSize(ssrReflectedUV, 0).xy;
  vec2 texCoord = gl_FragCoord.xy / texSize;

  vec4 uv = texture(ssrReflectedUV, texCoord);
  
  // Removes holes in the UV map.
  if (uv.b <= 0.0) {
    uv = vec4(0.0);
    float count = 0.0;

    for (int i = -size; i <= size; ++i) {
      for (int j = -size; j <= size; ++j) {
        uv    += texture
                  ( ssrReflectedUV
                  , ( (vec2(i, j) * separation)
                    + gl_FragCoord.xy
                    )
                    / texSize
                  );
        count += 1.0;
      }
    }

    uv.xyz /= count;
  }

  if (uv.b <= 0.0) { fragColor = vec4(0.0); return;}
  
  vec4 color = texture(gAlbedoSpec, uv.xy);
  float alpha = clamp(uv.b, 0.0, 1.0);
  color.a = alpha;
  fragColor = color;
}