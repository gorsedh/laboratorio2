//implementazioni resonancetype

#include "particletype.h"
#include "resonancetype.h"

#include <iostream>

void ResonanceType::Print() const {
    ParticleType::Print();
    std::cout << "Resonance width is " << fWidth_ << '\n';
}

double ResonanceType::GetWidth() const { return fWidth_; }

ResonanceType::ResonanceType(std::string name, double mass, int charge, double width) :
        ParticleType{name, mass, charge}, fWidth_{width} {};
