#include "Material.h"

Material::Material() {
this->type = Material_Diffuse_Blinn_Phong;
this->index_medium=1.;
this->transparency=0.;

this->diffuse_material= mlm::vec3(0.5,0.5,0.5);
this->kDiffuse=0.5;

this->specular_material=mlm::vec3(1.,1.,1.);
this->kSpecular=0.1;
this->indiceSpecular=2.;

}
Material::Material(mlm::vec3 color) {
this->type = Material_Diffuse_Blinn_Phong;
this->index_medium=1.;
this->transparency=0.;

this->diffuse_material= color;
this->kDiffuse=0.8;

this->specular_material=mlm::vec3(.2,.2,.2);
this->kSpecular=0.2;
this->indiceSpecular=100.;

}

