#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

#include <iostream>

int Particle::fNParticleType_ = 0;
ParticleType* Particle::fParticleType_[fMaxNumParticleType_];

Particle::Particle(std::string name, double fPx, double fPy, double fPz) : 
        fPx_{fPx}, fPy_{fPy}, fPz_{fPz} {
            fIParticle_ = FindParticle(name);
        };

int Particle::FindParticle(std::string name){
        for (int i = 0; i < fNParticleType_; i++) {
           if (fParticleType_[i]->GetParticleName() == name) { return i; }
        }
        std::cout << "Particle " << name << " not found" << '\n';
        return fNParticleType_ + 1;
};

void Particle::AddParticleType(std::string name, double mass, int charge) {

        int index = FindParticle(name);

        if (index < fNParticleType_) {
            std::cout << "Particle " << name << " already in the array\n";
            return;
        }

        if(fNParticleType_ < fMaxNumParticleType_) {
            fParticleType_[fNParticleType_] = new ParticleType(name, mass, charge);
            std::cout << "Particle " << name << " added to the array\n";
            fNParticleType_++;
            std::cout << "There are " << fMaxNumParticleType_ - fNParticleType_ << " spaces left in the array\n\n";
        } else {
            std::cout << "ARRAY IS FULL, DELETE SOME PARTICLES\n\n\n";
        }
};

void Particle::AddParticleType(std::string name, double mass, int charge, double width) {

        int index = FindParticle(name);
        
        if (index < fNParticleType_) {
            std::cout << "Particle " << name << " already in the array\n";
            return;
        }

        if(fNParticleType_ < fMaxNumParticleType_) {
            fParticleType_[fNParticleType_] = new ResonanceType(name, mass, charge, width);
            std::cout << "Particle " << name << " added to the array\n";
            fNParticleType_++;
            std::cout << "There are " << fMaxNumParticleType_ - fNParticleType_ << " spaces left in the array\n\n";
        } else {
            std::cout << "ARRAY IS FULL, DELETE SOME PARTICLES\n\n\n";
        }
};

int const Particle::GetIndex() { return fIParticle_; };
double const Particle::GetXMomentum() { return fPx_; };
double const Particle::GetYMomentum() { return fPy_; };
double const Particle::GetZMomentum() { return fPz_; };

void const Particle::ParticlePrint() {
    int ParticleIndex = GetIndex();
    std::cout << "Index is " << ParticleIndex << "\n";
    std::cout << "Particle type is " << fParticleType_[ParticleIndex]->GetParticleName() << "\n";
    std::cout << "Momentum of the particle is:\n x: " << GetXMomentum() << "\n y: " << GetYMomentum() 
                                          << "\n z: " << GetZMomentum() << "\n";
};

void Particle::SetParticle(int index) {
    if (index < fNParticleType_) { fIParticle_ = index; }
    else { std::cout << "No particle is associated to the index given in SetParticle()\n"; }
};

void Particle::SetParticle(std::string name) {
    int index = FindParticle(name);
    if (index < fNParticleType_) { fIParticle_ = index; }
    else { std::cout << "No particle is associated to the name given in SetParticle\n"; }
};

void Particle::PrintArray() {
    std::cout << "\n\033[0;36mArray of particles contains " << fNParticleType_ << " particles:\033[0m\n\n\033[1;33m";
    for (int i = 0; i < fNParticleType_; i++) {
         fParticleType_[i]->Print();
         std::cout << '\n';
    }
    std::cout << "\033[0m\n";
}
