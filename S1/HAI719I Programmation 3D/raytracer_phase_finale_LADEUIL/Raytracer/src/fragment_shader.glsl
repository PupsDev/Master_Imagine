#version 330 core

out vec4 FragColor;

in vec3 o_color;
in vec3 o_pos;

void main()
{


    FragColor = vec4(o_color, 1.0);

}
