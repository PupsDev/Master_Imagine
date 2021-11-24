class Sphere : public Object 
{
    private:
        Vec3 position;
        float rayon;

        Vec3 color;

 //r,g,b
        float kDiffuse;
        float kSpecular;
        float indiceSpecular;
        
    public:
        Sphere(Vec3 _position, float _rayon):position(_position), rayon(_rayon){
            color = Vec3 (1.0,0.0,0.0);
            kDiffuse = 0.5;
            kSpecular = 0.8;
            indiceSpecular=5.;
            
        }
        bool intersect(const Vec3 &orig, const Vec3 &dir, float &t) const 
        { 
            float t0, t1;
           
            Vec3 L = orig - position; 
            float a = dir*dir; 
            float b = 2 * (dir*L); 
            float c = L*L - rayon; 

            if (!solveQuadratic(a, b, c, t0, t1)) return false; 
            
            if (t0 > t1) std::swap(t0, t1); 
    
            if (t0 < 0) { 
                t0 = t1; // if t0 is negative, let's use t1 instead 
                if (t0 < 0) return false; // both t0 and t1 are negative 
            } 
    
            t = t0; 
            
            return true; 
    
        }
        Vec3 getPosition(){return position;}
        Vec3 getColor(){return color;}
        void setColor(Vec3 _color) {
        color = _color;
        }
        float getDiffuse(){return kDiffuse;}
        float getSpecular(){return kSpecular;}
        float getIndiceSpecular(){return indiceSpecular;}
};