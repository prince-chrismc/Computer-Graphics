#version 330 core

out vec4 color;

in vec3 col;

void main()
{
   if (col == vec3(0,0,0))
      color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
   else
      color = vec4(col, 1.0f);
}
