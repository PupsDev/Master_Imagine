#version 330 core

in vec4 vertex;
in vec2 UV;
// Ouput data
out vec3 color;
uniform sampler2D myTextureSampler2;


void main(){

        //color =vec3(0.2, 0.2,0.4);
        //color =vec3(abs(1./vertex.x), abs(1./vertex.y),abs(1./vertex.z));
        
        color = texture( myTextureSampler2, UV ).rgb;

}
