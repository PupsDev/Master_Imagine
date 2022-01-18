#version 330 core

// Output data
out vec4 FragColor;
// Ajouter une variable interpolée o_color venant du vertex shader
//
in vec3 o_color;
in vec2 TexCoord;

uniform sampler2D color;
uniform sampler2D metal;
uniform sampler2D normal;
uniform sampler2D rough;

void main()
{

    // Mettre à jour la couleur avec la variable interpolée venant du vertex shader
    //FragColor = o_color;// Output color = red

    FragColor = mix(texture(color, TexCoord),texture(normal, TexCoord),0.5) ;

}
