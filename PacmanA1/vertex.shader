#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 col;

out vec3 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);

   if(col.x > 0.1f && col.y > 0.1f && col.z > 0.1f)
      color = col;
   else
      color = vec3(0.2f, 0.3f, 1.0f);
}
