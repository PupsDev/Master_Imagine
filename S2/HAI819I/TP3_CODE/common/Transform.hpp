#ifndef TRANSFORM
#define TRANSFORM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtx/rotate_vector.hpp>

class Transform
{
    glm::mat3 m;
    glm::vec3 translation;

    glm::vec3 applyToPoint(glm::vec3 p)
    {
        return m * p +translation;
    }
    
    glm::vec3 applyToVector(glm::vec3 v)
    {
        return m * v;
    }
};
#endif