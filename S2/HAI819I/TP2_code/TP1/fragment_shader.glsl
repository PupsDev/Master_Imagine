#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler[6];


void main(){
        
        float factor = texture(myTextureSampler[3], UV ).r ;
        if(factor< 0.1){
                color = mix( texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).rgb,factor);

        }
        else if (factor < 0.5)
        {
                color = mix(texture( myTextureSampler[1], UV ).rgb,texture( myTextureSampler[2], UV ).rgb,factor);
        }
        else
        {
                color = texture( myTextureSampler[2], UV ).rgb;

        }

}
