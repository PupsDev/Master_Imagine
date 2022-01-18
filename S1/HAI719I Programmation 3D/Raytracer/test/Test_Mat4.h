#include <cassert>
#include "mlm/mat4.h"

using namespace mlm;

class TEST_MAT4{
    public:
    TEST_MAT4()
    {
        std::cout<<"Test Mat4"<<std::endl;
        /*
        mlm::mat4 C;
        glm::mat4 C2;
        glm::mat4 B2 = glm::mat4(1.,2.,3.,4.,
                                1.,2.,3.,4.,
                                1.,2.,3.,4.,
                                1.,2.,3.,4.);
        clock_t start, stop;
        int i;
        start = clock();
        for( i = 0; i < 1000000;i++)
            mlm::mat4 C = B.multiply(B);
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
        mlm::mat4 id = mlm::identity();
        mlm::mat4 B = mlm::mat4(1.,2.,3.,1.,
                                1.,2.,3.,1.,
                                1.,2.,3.,1.,
                                1.,2.,3.,1.);

        mlm::vec4 Vec = mlm::vec4(1.,1.,1.,1.);
        
        mlm::mat4 myScale = mlm::scale(2.);
        mlm::mat4 myXRotation = mlm::rotationX(90.);
        mlm::mat4 myTranslation = mlm::translation(mlm::vec3(1.,1.,1.));

        Vec = mlm::multiply(myScale,Vec);
        cout<<Vec<<endl;
        Vec = mlm::multiply(myXRotation,Vec);
        cout<<Vec<<endl;
        Vec = mlm::multiply(myTranslation,Vec);
        cout<<Vec<<endl;
        

    }

};