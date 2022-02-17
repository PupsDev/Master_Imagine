#version 330 core

in vec4 vertex;
in vec3 vertexLocal;
in vec2 UV;

out vec3 color;
uniform sampler2D myTextureSampler[8];


void main(){
        
        float factor = texture(myTextureSampler[5], UV ).r;
        float t0 = 0.44;
        float t1 = 0.47;
        float t2 = 0.64;
        float t3 = 0.84;
        float t4 = 0.98;


        //color = texture( myTextureSampler[6], UV ).gbr + step(factor,t1)*mix()
        if(factor< t0){
                color = texture( myTextureSampler[4], UV ).rgb;
        }
        else if(factor< t1){
                color = mix(texture(myTextureSampler[4],UV).rgb,texture( myTextureSampler[6], UV ).gbr,factor);
        }
        else if(factor< t2){
                
                color = texture( myTextureSampler[0], UV ).rgb;//*/mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).rgb,      (factor-t2)/(t3-t2)  );
                color = mix(color,texture(myTextureSampler[7],UV).rgb,0.5);
        }
        else if (factor < t3)
        {
               color = mix(texture( myTextureSampler[0], UV ).rgb,texture( myTextureSampler[1], UV ).gbr,(factor-t2)/(t3-t2));//texture( myTextureSampler[1], UV ).gbr;//mix(texture( myTextureSampler[0], UV ).grb,texture( myTextureSampler[1], UV ).grb, (factor-t2)/(t3-t2));
               color = mix(color,texture(myTextureSampler[7],UV).rgb,0.5);
        }


        else if (factor < t4)
        {
                color = mix(texture( myTextureSampler[1], UV ).gbr,texture( myTextureSampler[2], UV ).rbg,(factor-t3)/(t4-t3));//mix(texture( myTextureSampler[1], UV ).rgb,texture( myTextureSampler[2], UV ).rgb, (factor-t3)/(t3-t2));
        }
        else
        {
                color = vec3(1.,1.,1.);

        }

}
