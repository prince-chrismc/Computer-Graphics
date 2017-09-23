#version 330 core

out vec4 color;
uniform int object_type;
in vec3 col;

void main()
{
   color = (object_type == 0) ? vec4(1.0f, 0.0f, 0.0f, 1.0f) : (object_type == 1) ? vec4(0.0f, 1.0f, 0.0f, 1.0f) : \
           (object_type == 2) ? vec4(0.0f, 0.0f, 1.0f, 1.0f) : vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
