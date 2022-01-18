
#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

uniform float scale;
uniform mat4 translate;
uniform mat4 projection;
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexTex_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
//------

out vec3 o_color;
out vec2 TexCoord;

out vec3 Fpos;
out vec3 Fnor;



void main(){

    gl_Position =  projection*translate*vec4(vertexPosition_modelspace, 1.0);

    Fpos = (translate*vec4(vertexPosition_modelspace, 1.0)).xyz; 
    Fnor = vertexNormal_modelspace;

    TexCoord = vec2(vertexTex_modelspace.x, vertexTex_modelspace.y);
}

