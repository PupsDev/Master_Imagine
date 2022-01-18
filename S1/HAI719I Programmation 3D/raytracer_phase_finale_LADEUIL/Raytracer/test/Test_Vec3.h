#include <cassert>
#include "mlm/vec3.h"

using namespace mlm;

class TEST_VEC3{
    public:
    TEST_VEC3()
    {
        std::cout<<"Test Vec3"<<std::endl;
        //Constant
        vec3 zeroVec=vec3(0.,0.,0.);
        vec3 aBefore=vec3(1.,2.,3.);
        vec3 bBefore=vec3(1.,2.,3.);
        vec3 doubleA = vec3(2.,4.,6.);
        vec3 aPlusC = vec3(3.,4.,5.);
        vec3 aMinusC = vec3(-1.,0.,1.);
        float c = 2.;

        // Variable
        vec3 a=vec3(1.,2.,3.);
        vec3 b=vec3(1.,2.,3.);

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