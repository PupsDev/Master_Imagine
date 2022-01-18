class Ray
{
    private:
     Vec3 origin;
     Vec3 direction;


    public:
        Ray(Vec3 _origin, Vec3 _direction):origin(_origin), direction(_direction){}

        Vec3 at(float t)
        {
            return origin + direction * t; 
        }
        Vec3 GetOrigin() const {
            return origin;
        }

        void SetOrigin(Vec3 origin) {
            origin = origin;
        }
        Vec3 GetDirection() const {
          return direction;
        }

        void SetDirection(Vec3 direction) {
            direction = direction;
        }

};