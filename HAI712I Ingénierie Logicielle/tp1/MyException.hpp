#include <iostream>
class MyException : public std::string
{
public:
    MyException(const int err) : int(err) {}       
};