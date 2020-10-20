#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include <iostream>

class ResonanceType : public ParticleType{
public:
    double const GetWidth(); //getter

    void const Print() override; //print

    ResonanceType(std::string name, double mass, int charge, double width); //costruttore

private: //attributo di resonance
    double const fWidth_;
};

#endif