#include "IDictionary.hpp"
#ifndef DEF_ORDERED
#define DEF_ORDERED

class OrderedDictionary : public IDictionary {
    public:
     OrderedDictionary()
     {
        currentSize=0;
     }

    
};
#endif