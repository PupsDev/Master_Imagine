#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler[7];


void main(){
        
        float factor = texture(myTextureSampler[5], UV ).r;
        float t1 = 0.37;
        float t2 = 0.56;
        float t3 = 0.58;
        float t4 = 0.69;


        //color = texture( myTextureSampler[6], UV ).gbr + step(factor,t1)*mix()
        if(factor< 0.37){
                color =   texture( myTextureSampler[6], UV ).gbr;

        }
        else if(factor< 0.45){
                color = texture( myTextureSampler[0], UV ).rgb;//mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).rgb,      (factor-t1)/(t2-t1)  );

        }
        else if (factor < 0.58)
        {
               color = mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).gbr,factor);//texture( myTextureSampler[1], UV ).gbr;//mix(texture( myTextureSampler[0], UV ).grb,texture( myTextureSampler[1], UV ).grb, (factor-t2)/(t3-t2));
        }
        else if (factor < 0.69)
        {
                color = mix(texture( myTextureSampler[1], UV ).gbr,texture( myTextureSampler[2], UV ).rbg,factor);//mix(texture( myTextureSampler[1], UV ).rgb,texture( myTextureSampler[2], UV ).rgb, (factor-t3)/(t3-t2));
        }
        else
        {
                color = vec3(1.,1.,1.);

        }

}
