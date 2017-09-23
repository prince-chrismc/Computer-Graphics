#version 330 core

out vec4 color;
uniform int object_type;

void main()
{
   color = (object_type == 0) ? vec4(1.0f, 0.0f, 0.0f, 1.0f) : // red
           (object_type == 1) ? vec4(0.0f, 1.0f, 0.0f, 1.0f) : // green
           (object_type == 2) ? vec4(0.0f, 0.0f, 1.0f, 1.0f) : // blue
           (object_type == 3) ? vec4(0.5f, 0.5f, 0.5f, 1.0f) : // grey
           (object_type == 4) ? vec4(1.0f, 0.9333f, 0.0f, 1.0f) : // pacman yellow
           (object_type == 5) ? vec4(0.0f, 0.5f, 0.5f, 1.0f) : // teal
           vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
