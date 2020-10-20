//implementazioni resonancetype

#include "particletype.h"
#include "resonancetype.h"

#include <iostream>

void const ResonanceType::Print() {
    ParticleType::Print();
    std::cout << "Particle widht is " << fWidth_ << '\n';
}

ResonanceType::ResonanceType(std::string name, double mass, int charge, double width) :
        ParticleType{name, mass, charge}, fWidth_{width} {};
