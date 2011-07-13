#ifndef ENTITY_H__
#define ENTITY_H__

#include <string>
#include <vector>

#include <iostream>

#include <boost/shared_ptr.hpp>

//using namespace puddle;

namespace poleng
{

namespace bonsai
{

namespace puddle
{

typedef boost::shared_ptr<std::string> StringPtr;

class Entity;

typedef std::vector<Entity*> Entities;

class Entity
{
    public:
        Entity();
        ~Entity();
        virtual std::string getType() { return type; }
        virtual std::string getCompiled();
        virtual std::string getId() { return id; }
        virtual void updateCompiled() {}
        virtual std::string toXml();
    private:
        std::string id;
        std::string type;
        Entities children;
};

}

}

}

#endif
