#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

#include <iostream>

class ParticleType
{
public:
    std::string GetParticleName() const; //i getter
    double GetMass() const;
    int GetCharge() const;
    virtual double GetWidth() const;

    virtual void Print() const; //print

    ParticleType(std::string name, double mass, int charge); //costruttore

private: //attributi
    std::string const fName_;
    double const fMass_;
    int const fCharge_;
};

#endif