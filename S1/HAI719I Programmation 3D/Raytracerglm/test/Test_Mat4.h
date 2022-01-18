#include <cassert>
#include "glm/mat4x4.hpp"

using namespace glm;

class TEST_MAT4{
    public:
    TEST_MAT4()
    {
        std::cout<<"Test Mat4"<<std::endl;
        /*
        glm::mat4 C;
        glm::mat4 C2;
        glm::mat4 B2 = glm::mat4(1.,2.,3.,4.,
                                1.,2.,3.,4.,
                                1.,2.,3.,4.,
                                1.,2.,3.,4.);
        clock_t start, stop;
        int i;
        start = clock();
        for( i = 0; i < 1000000;i++)
            glm::mat4 C = B.multiply(B);
        stop = clock();
        std::cout<<(double)(stop - start) / CLOCKS_PER_SEC<<std::endl;
        std::cout<<C<<std::endl;

        start = clock();
        for( i = 0; i < 1000000;i++)
            glm::mat4 C = B2*B2;
        stop = clock();
        for(int i =0; i <16;i++)
            std::cout<<*(value_ptr(C2)+i)<<std::endl;
        std::cout<<(double)(stop - start) / CLOCKS_PER_SEC<<std::endl;
        */
           //TEST_VEC4();
        //TEST_VEC3();
        glm::mat4 id = glm::identity();
        glm::mat4 B = glm::mat4(1.,2.,3.,1.,
                                1.,2.,3.,1.,
                                1.,2.,3.,1.,
                                1.,2.,3.,1.);

        glm::vec4 Vec = glm::vec4(1.,1.,1.,1.);
        
        glm::mat4 myScale = glm::scale(2.);
        glm::mat4 myXRotation = glm::rotationX(90.);
        glm::mat4 myTranslation = glm::translation(glm::vec3(1.,1.,1.));

        Vec = glm::multiply(myScale,Vec);
        cout<<Vec<<endl;
        Vec = glm::multiply(myXRotation,Vec);
        cout<<Vec<<endl;
        Vec = glm::multiply(myTranslation,Vec);
        cout<<Vec<<endl;
        

    }

};