#include <cassert>
#include "glm/vec4.hpp"

using namespace glm;

class TEST_VEC4
{
    public:
    TEST_VEC4()
    {
        std::cout<<"Test Vec4"<<std::endl;
        //Constant
        vec4 zeroVec=vec4(0.,0.,0.,0.);
        vec4 aBefore=vec4(1.,2.,3.,4.);
        vec4 bBefore=vec4(1.,2.,3.,4.);
        vec4 doubleA = vec4(2.,4.,6.,8.);
        vec4 aPlusC = vec4(3.,4.,5.,6.);
        vec4 aMinusC = vec4(-1.,0.,1.,2.);
        float c = 2.;

        // Variable
        vec4 a=vec4(1.,2.,3.,4.);
        vec4 b=vec4(1.,2.,3.,4.);

        // Addition

        assert(a+b==doubleA);
        std::cout << "Test OK !\n";

        assert(a+c==aPlusC);
        std::cout << "Test OK !\n";

        assert(c+a==aPlusC);
        std::cout << "Test OK !\n";


        assert((a+=b)==doubleA);
        std::cout << "Test OK !\n";
        assert(a==doubleA);
        std::cout << "Test OK !\n";
        
       
        a = aBefore;
        /*
        std::cout<<a<<std::endl;
        std::cout<<c<<std::endl;
        std::cout<<(a+c)<<std::endl;
        std::cout<<(a+=c)<<std::endl;
        */
        
        assert((a+=c)==aPlusC);
        std::cout << "Test OK !\n";

        assert(a==aPlusC);
        std::cout << "Test OK !\n";
        a = aBefore;
        // Substraction

        assert(a-b==zeroVec);
        std::cout << "Test OK !\n";

        assert(a-c==aMinusC);
        std::cout << "Test OK !\n";

        assert(c-a==aMinusC);
        std::cout << "Test OK !\n";


        assert((a-=b)==zeroVec);
        std::cout << "Test OK !\n";
        assert(a==zeroVec);
        std::cout << "Test OK !\n";
        
        a = aBefore;

        assert((a-=c)==aMinusC);
        std::cout << "Test OK !\n";

        assert(a==aMinusC);
        std::cout << "Test OK !\n";
        a = aBefore;
 
        // Multiplication

        assert((a*c)==doubleA);
        std::cout << "Test OK !\n";

        assert((c*a)==doubleA);
        std::cout << "Test OK !\n";

        assert((a*=c)==doubleA);
        std::cout << "Test OK !\n";

        assert((a)==doubleA);
        std::cout << "Test OK !\n";

        // Division
        assert((a/c)==aBefore);
        std::cout << "Test OK !\n";

        assert((a/=c)==aBefore);
        std::cout << "Test OK !\n";

        assert(a==aBefore);
        std::cout << "Test OK !\n";
         a = aBefore;





        

    }

};