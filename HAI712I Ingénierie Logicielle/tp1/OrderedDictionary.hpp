#include "IDictionary.hpp"
#ifndef DEF_ORDERED
#define DEF_ORDERED


template <class Object> 
class OrderedDictionary : public IDictionary<Object> {
    public:
     OrderedDictionary()
     {
         this->key = new Object[0];
         this->value = new Object[0];
         this->currentSize=0;
     }
      int indexOf(Object _key)
      {
            int currentKey = -1;
            
            for(int i = 0 ; i< this->currentSize+1; i++)
            {

               if(_key == this->key[i] )
                  currentKey = i;
            } 
            return currentKey;
      }
      int newIndexOf(Object _key)
      {
            int index =-1;
            if(indexOf(_key)==-1)
            {
               index = this->currentSize;
               Object * keyTmp = new Object[index +1];
               Object * valueTmp = new Object[index+1];

               for(int i=0; i < index ; i++)
               {
                  keyTmp[i] = this->key[i];
                  valueTmp[i] = this->value[i];
               }

               this->key = keyTmp;
               this->value = valueTmp;
               this->currentSize++;

            }
            
            return index;
      }
      int size()
      {
            return this->currentSize;
      }
      void printDico()
      {
         std::cout<<"Taille dictionnaire : "<<this->currentSize<<std::endl;
         for(int i=0; i < this->currentSize ; i++)
         {

            std::cout<<"("<<this->key[i]<<","<<this->value[i]<<")"<<std::endl;

         }

      }

    
};
#endif