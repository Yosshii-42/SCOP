#version 330 core
out vec4      fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform bool useTexture;      // Texture 切替操作用

void  main()
{
  if (useTexture)
    fragColor = texture(ourTexture, TexCoord);
  else
    fragColor = vec4(ourColor, 1.0);
}
