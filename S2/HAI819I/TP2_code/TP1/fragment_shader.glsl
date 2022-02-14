#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler[6];


void main(){
        
        float factor = texture(myTextureSampler[4], UV ).r;
        if(factor< 0.33){
                color = vec3(0.,0.,1.);

        }
        else if(factor< 0.66){
                color = mix(  vec3(0.,0.,1.),texture( myTextureSampler[0], UV ).rgb,factor);

        }
        else if (factor < 0.9)
        {
                color = mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).rgb,factor);
        }
        else
        {
                color = mix(texture( myTextureSampler[1], UV ).rgb,texture( myTextureSampler[2], UV ).rgb,factor);

        }

}
