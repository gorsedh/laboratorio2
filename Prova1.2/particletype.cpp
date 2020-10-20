//implementazioni particletype

#include "particletype.h"

#include <iostream>


std::string const ParticleType::GetParticleName() { return fName_; };
double const ParticleType::GetMass() { return fMass_; };
int const ParticleType::GetCharge() { return fCharge_; };

void const ParticleType::Print() {
    std::cout << "Particle name is " << fName_ << '\n';
    std::cout << "Particle mass is " << fMass_ << '\n';
    std::cout << "Particle charge is " << fCharge_ << '\n';
}

ParticleType::ParticleType(std::string name, double mass, int charge) :
         fName_{name}, fMass_{mass}, fCharge_{charge} {};