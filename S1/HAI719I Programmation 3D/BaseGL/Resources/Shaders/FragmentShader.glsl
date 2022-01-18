#version 450 core // Minimal GL version support expected from the GPU

struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int isActive;
    mat4 depthMap;
};

const int nb_light = 1;
uniform LightSource lightSources[nb_light];


struct Material {
    vec3 albedo;
    float shininess;
};

uniform Material material;

in vec3 fPosition; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)
in vec3 fPositionWorldSpace;
in vec3 fNormal;
in vec2 fTexCoord;

out vec4 colorResponse; // Shader output: the color response attached to this fragment

uniform mat4 projectionMat, modelViewMat, normalMat;

uniform sampler2D shadowMap[nb_light];

float pi = 3.1415927;

float ShadowCalculation(vec4 fragPosLight, int i)
{
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    projCoords = projCoords * 0.5 +0.5;
    float closestDepth = texture( shadowMap[i], fTexCoord.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth > (closestDepth) ? 1.0:0.0;
    return shadow;
}
void main() {
    vec3 n = normalize(fNormal);

    // Linear barycentric interpolation does not preserve unit vectors
    vec3 wo = normalize (-fPosition); // unit vector pointing to the camera
    vec3 radiance = vec3(0,0,0);

    float bias = 0.005;

    if( dot( n , wo ) >= 0.0 ) {
        {
            for(int i = 0 ; i < nb_light ;i ++)
            {
                if( lightSources[i].isActive == 1 ) { // WE ONLY CONSIDER LIGHTS THAT ARE SWITCHED ON
                 {

                    vec3 wi = normalize ( vec3((modelViewMat * vec4(lightSources[i].position,1)).xyz) - fPosition ); // unit vector pointing to the light source (change if you use several light sources!!!)
                    if( dot( wi , n ) >= 0.0 ) { // WE ONLY CONSIDER LIGHTS THAT ARE ON THE RIGHT HEMISPHERE (side of the tangent plane)
                        vec3 wh = normalize( wi + wo ); // half vector (if wi changes, wo should change as well)
                        vec3 Li = lightSources[i].color * lightSources[i].intensity;
                        vec4 ShadowCoord = 0.1*lightSources[i].depthMap*vec4(fPositionWorldSpace,1.);
                        float visibility = ShadowCalculation(ShadowCoord,i);
                        radiance = radiance +
                                (1.-visibility)*Li // light color
                                *material.albedo
                                * ( max(dot(n,wi),0.0) + pow(max(dot(n,wh),0.0),material.shininess) )
                                ;
                    }
                }

            }

            }
        }
    }
    colorResponse = vec4 (radiance, 1.0); // Building an RGBA value from an RGB one.
}




