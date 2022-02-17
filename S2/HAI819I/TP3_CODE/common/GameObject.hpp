#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "common/Transform.hpp"
#include <memory>
#include <list>
#include <vector>

class Component;

class GameObject
{


    public:


    protected:
        Transform* transformation;
        std::list<  Component * > components;
};
#endif
