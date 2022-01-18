
#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader

// Input vertex data, different for all executions of this shader.

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor_modelspace;

// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
//------

out vec3 o_color;
out vec3 o_pos;
out vec3 o_norm;




void main(){
    

    gl_Position =  vertexPosition_modelspace;
    o_color= vec3((vertexColor_modelspace).x,(vertexColor_modelspace).y,(vertexColor_modelspace).z);
    o_pos = vec3((vertexPosition_modelspace).x,(modelMatrix*vertexPosition_modelspace).y,(modelMatrix*vertexPosition_modelspace).z);



}

