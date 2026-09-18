#version 330 core
out vec4      fragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void  main()
{
  fragColor = texture(ourTexture, TexCoord);
//  fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
