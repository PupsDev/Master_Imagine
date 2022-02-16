#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertex;
out vec3 vertexLocal;
out vec2 UV;

uniform sampler2D myTextureSampler[7];
out vec3 heightmap;

void main(){
        vec3 heightmap=vec3(0.,0.,0.);
        heightmap.z = 35.*(texture(myTextureSampler[5], vertexUV ).r);

        /*if( texture(myTextureSampler[5], vertexUV ).r< 0.37){
                gl_Position =  projection*view*model* vec4(vertices_position_modelspace-texture(myTextureSampler[5], vertexUV ).r,1);
        }
        else*/
                gl_Position =  projection*view*model* vec4(vertices_position_modelspace+heightmap,1);
        vertex = gl_Position;
        vertexLocal = vertices_position_modelspace;
        UV = vertexUV;



}

