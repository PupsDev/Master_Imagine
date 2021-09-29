#include <iostream>

#include "OrderedDictionary.hpp"
#include "SortedDictionary.hpp"
#include "FastDictionary.hpp"


using namespace std;

int main()
{
    OrderedDictionary<string>* dicoOrder = new OrderedDictionary<string>();
    dicoOrder->putValue("premier", "hello");
    dicoOrder->putValue("premier2", "hello2");
    dicoOrder->printDico();
    
    cout<<dicoOrder->getValue("premier")<<endl;
    cout<<dicoOrder->hasKey("premier")<<endl;
    cout<<dicoOrder->size()<<endl;
    
    return 0;
}