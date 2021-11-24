class Camera
{
    private:
        Ray* ray;
        float nearClip;
        float farClip;
        float fov;
        float scale;

    public:
        Camera(Ray* camera)
        {
                this->nearClip=5.;
                this->farClip=100.;
                this->fov = 150.;
                this->scale = tan((this->fov * 0.5 )* M_PI / 180);
                 

                this->ray = camera;
                
        }
        Ray* GetRay() const {
                return ray;
        }

        void SetRay(Ray* ray) {
                this->ray = ray;
        }

        float GetNearClip() const {
                return nearClip;
        }

        void SetNearClip(float nearClip) {
                nearClip = nearClip;
        }

        float GetFarClip() const {
                return farClip;
        }

        void SetFarClip(float farClip) {
                farClip = farClip;
        }

        float GetFov() const {
                return fov;
        }

        void SetFov(float fov) {
                fov = fov;
        }

        float GetScale() const {
                return scale;
        }

        void SetScale(float scale) {
                scale = scale;
        }


};