#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

#include <iostream>

class ParticleType{
public:
    std::string const GetParticleName(); //i getter
    double const GetMass();
    int const GetCharge();

    virtual void const Print(); //print

    ParticleType(std::string name, double mass, int charge); //costruttore

private: //attributi
    std::string const fName_;
    double const fMass_;
    int const fCharge_;
};

#endif