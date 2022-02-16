#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler[7];


void main(){
        
        float factor = texture(myTextureSampler[5], UV ).r;
        float t1 = 0.25;
        float t2 = 0.33;
        float t3 = 0.66;
        if(factor< 0.25){
                color =   texture( myTextureSampler[6], UV ).gbr;

        }
        else if(factor< 0.33){
                color =texture( myTextureSampler[0], UV ).rgb;

        }
        else if (factor < 0.66)
        {
                color = mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).rgb,factor);
        }
        else
        {
                color = mix(texture( myTextureSampler[1], UV ).rgb,texture( myTextureSampler[2], UV ).rgb,factor);

        }

}
