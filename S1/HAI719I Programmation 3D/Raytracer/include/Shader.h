#ifndef SHADER_H
#define SHADER_H
#include "mlm/mat4.h"

class Shader
{
    public:
        Shader();
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();
        void setUniformMat4(const GLchar* name, mlm::mat4 matrix);
        void enable() const;
        void disable() const;
        GLuint load(const char* vertPath, const char* fragPath);
    private:
        GLuint shaderId;
        const char * vertPath;
        const char * fragPath;

       

};
#endif