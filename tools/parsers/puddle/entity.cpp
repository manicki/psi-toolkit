
#include "entity.hpp"

namespace poleng
{

namespace bonsai
{
    namespace puddle
    {

Entity::Entity()
{ 
    type = "unknown"; 
    id = ""; 
    //children = new Entities; 
}

Entity::~Entity()
{ 
   // delete children; 
//    std::cerr << "destrukcja entity" << std::endl; 
}

std::string Entity::getCompiled()
{
    return "<<>";
}

std::string Entity::toXml()
{
    return "<nothing/>";
}

}

}

}
