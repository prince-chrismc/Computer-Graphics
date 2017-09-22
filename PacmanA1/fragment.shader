#version 330 core

out vec4 color;
in vec3 col;

void main()
{
   //color = vec4(1.0f, 0.5f, 0.2f, 1.0f); // cmc-edit : original fixed orange
   color = vec4(col, 1.0f);
}
