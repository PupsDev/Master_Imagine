
enum MaterialType {
    Material_Diffuse_Blinn_Phong ,
    Material_Glass,
    Material_Mirror
};
class Material 
{ 
    private:
        Vec3 color;
        float index_medium;
        float transparency;

        Vec3 diffuse_material;
        float kDiffuse;

        Vec3 specular_material;
        float kSpecular;
        float indiceSpecular;

        MaterialType type;

    public:
        Material() {
            this->type = Material_Diffuse_Blinn_Phong;
            this->index_medium=1.;
            this->transparency=0.;

            this->diffuse_material= Vec3(0.5,0.5,0.5);
            this->kDiffuse=0.5;

            this->specular_material=Vec3(1.,1.,1.);
            this->kSpecular=0.8;
            this->indiceSpecular=100;

        }
                float GetIndex_medium() const {
                return index_medium;
        }

        void SetIndex_medium(float index_medium) {
                this->index_medium = index_medium;
        }

        float GetTransparency() const {
                return transparency;
        }

        void SetTransparency(float transparency) {
                this->transparency = transparency;
        }

        Vec3 GetDiffuse_material() const {
                return diffuse_material;
        }

        void SetDiffuse_material(Vec3 diffuse_material) {
                this->diffuse_material = diffuse_material;
        }

        float GetKDiffuse() const {
                return kDiffuse;
        }

        void SetKDiffuse(float kDiffuse) {
                this->kDiffuse = kDiffuse;
        }

        Vec3 GetSpecular_material() const {
                return specular_material;
        }

        void SetSpecular_material(Vec3 specular_material) {
                this->specular_material = specular_material;
        }

        float GetKSpecular() const {
                return kSpecular;
        }

        void SetKSpecular(float kSpecular) {
                this->kSpecular = kSpecular;
        }

        float GetIndiceSpecular() const {
                return indiceSpecular;
        }

        void SetIndiceSpecular(float indiceSpecular) {
                this->indiceSpecular = indiceSpecular;
        }

        MaterialType GetType() const {
                return type;
        }

        void SetType(MaterialType type) {
                this->type = type;
        }

}; 