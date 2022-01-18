#ifndef SHADER_H
#define SHADER_H
#include "glm/mat4x4.hpp"

class Shader
{
    public:
        Shader();
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();
        void setUniformMat4(const GLchar* name, glm::mat4 matrix);
        void enable() const;
        void disable() const;
        GLuint load(const char* vertPath, const char* fragPath);
    private:
        GLuint shaderId;
        const char * vertPath;
        const char * fragPath;

       

};
#endif