
 bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr == 0) x0 = x1 = - 0.5 * b / a; 
    else { 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(discr)) : 
            -0.5 * (b - sqrt(discr)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
    if (x0 > x1) std::swap(x0, x1); 
 
    return true; 
} 
float clip(float n, float lower, float upper) {
  if(n < lower)return lower;
  if(n>upper)return upper;
  return n;
    
}
enum ObjectType {
    SPHERE,
    MESH
};

class Object 
{
    private :
    Material *material;
    ObjectType type;





    public: 
        // virtual intersect function, needs to be overloaded by derived class
        virtual bool intersect(const Vec3 &orig, const Vec3 &dir, float &t) const = 0;
        virtual Vec3 getPosition()=0;

        virtual Vec3 getColor()=0;
        virtual void setColor(Vec3 _color)=0;
        virtual float getDiffuse()=0;
        virtual float getSpecular()=0;
        virtual float getIndiceSpecular()=0;

        Material* getMaterial()const
        {
            return this->material;
        }
        void setMaterial(Material *material)
        {
            this->material=material;
        }
        ObjectType GetType() const {
        return type;
        }
        void SetType(ObjectType type) {
        type = type;
        }
        
        virtual ~Object() {} // virtual destructor 
        Object() {} // constructor 
}; 