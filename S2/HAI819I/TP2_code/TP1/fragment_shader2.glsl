#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler2;


void main(){
        color = texture( myTextureSampler2, UV ).rgb;

}
