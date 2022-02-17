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

uniform sampler2D myTextureSampler[8];
out vec3 heightmap;

void main(){

        gl_Position =  projection*view*model* vec4(vertices_position_modelspace,1);
        vertex = gl_Position;
        vertexLocal = vertices_position_modelspace;
        UV = vertexUV;



}

