#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;

uniform mat4 transform;

void  main()
{
//  vec4 pos = vec4(
//      (aPos.z - 1.4) * 0.65,
//      (aPos.y + 0.05) * 0.65,
//      aPos.x * 0.65,
//      1.0
//  );

  gl_Position = transform * vec4(aPos, 1.0);
//  defaultColor = aColor;
}
