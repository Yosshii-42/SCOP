#version 330 core

out vec4          fragColor;

in vec3           ourColor;
in vec2           TexCoord;

uniform sampler2D ourTexture;
uniform bool      useTexture; // Texture 切替操作用
uniform float     textureBlend;

void  main()
{
  vec4 grayColor = vec4(ourColor, 1.0);
  vec4 texColor = texture(ourTexture, TexCoord);

  fragColor = mix(grayColor, texColor, textureBlend);
}
