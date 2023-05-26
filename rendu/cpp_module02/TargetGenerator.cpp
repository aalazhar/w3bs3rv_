#include "TargetGenerator.hpp"

TargetGenerator::~TargetGenerator(){
    std::map<std::string, ATarget *>::iterator it = _map.end();
    for(;it != this->_map.end(); it++)
        delete it->second;
    this->_map.clear();
}

TargetGenerator::TargetGenerator(){
    
}



void TargetGenerator::learnTargetType(ATarget *t){
    if (_map.find(t->getType()) == _map.end())
        _map.insert(make_pair(t->getType(), t->clone()));
}

void TargetGenerator::forgetTargetType(std::string const &s){
    std::map<std::string, ATarget *>::iterator it = _map.find(s);
    if (it != _map.end())
        delete it->second;
    _map.erase(s);

}

ATarget* TargetGenerator::createTarget(std::string const &s){
    if ( _map.find(s) == _map.end() )
        return NULL;
    return _map[s];
}