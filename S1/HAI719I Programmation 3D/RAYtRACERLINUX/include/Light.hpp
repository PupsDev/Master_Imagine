
enum LightType {
    LightType_Point,
    LightType_Regular_Area,
    LightType_Random_Area
};
class Light
{
    private:
        Vec3 position;
        float intensity;
        float intensitySpec;
        LightType type;

        Vec3 v1;
        Vec3 v2;


    public:
        Light(Vec3 _position, float _intensity,LightType _lightype):position(_position), intensity(_intensity),type( _lightype){}
       
        Vec3 getPosition(){return position;}
        LightType getType(){return type;}
        void setPosition( Vec3 position){this->position=position;}
        float getIntensity(){return intensity;}
        float GetIntensitySpec() const {
        return intensitySpec;
        }

        void SetIntensitySpec(float _intensitySpec) {
        intensitySpec = _intensitySpec;
        }
        Vec3 getV1(){return v1;}
        Vec3 getV2(){return v2;}

        void setV1(Vec3 v){v1=v;}
        void setV2(Vec3 v){v2=v;}
        
};
