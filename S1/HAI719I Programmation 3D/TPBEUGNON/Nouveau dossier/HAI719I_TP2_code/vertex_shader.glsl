#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

uniform float scale;
uniform mat4 translate;
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor_modelspace;
layout(location = 2) in vec2 vertexTex_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
//------

out vec3 o_color;
out vec2 TexCoord;

void main(){

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    //vertexPosition_modelspace = scale * vertexPosition_modelspace;
    //vertexPosition_modelspace + = translate;
    gl_Position =  translate*vec4(vertexPosition_modelspace, 1.0);;

    //Assigner la normale à la variable interpolée color
    //ajouter ici
    o_color =  vertexColor_modelspace;
    TexCoord = vec2(vertexTex_modelspace.x, vertexTex_modelspace.y);
}
